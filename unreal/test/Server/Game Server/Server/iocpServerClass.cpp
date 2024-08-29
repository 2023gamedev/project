#pragma once


#include"iocpServerClass.h"

// BT
#include "Task.h"
#include "Selector.h"
#include "Sequence.h"

#include "CanSeePlayer.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"
#include "CanNotAttack.h"
#include "CanAttack.h"
#include "MoveTo.h"
#include "Attack.h"



std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
std::unordered_map<int, Player_Location> playerLocations;

IOCP_CORE::IOCP_CORE()
{	
	playerIndex = 0;
	timer_thread = thread(&IOCP_CORE::Timer_Thread, this);

	zombie_thread = thread(&IOCP_CORE::Zombie_BT_Thread, this);

	//Nodes = nodeclass->LoadNodesFromFile();

	zombieclass = new ZombieController();

	IOCP_GetServerIpAddress();
	CheckThisCPUcoreCount();

	IOCP_Initialize();
	
	IOCP_MakeWorkerThreads();
}

IOCP_CORE::~IOCP_CORE()
{
	WSACleanup();
}

void IOCP_CORE::IOCP_GetServerIpAddress()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	PHOSTENT hostinfo;
	char hostname[50];
	char ipaddr[50];
	memset(hostname, 0, sizeof(hostname));
	memset(ipaddr, 0, sizeof(ipaddr));

	int nError = gethostname(hostname, sizeof(hostname));
	if (nError == 0)
	{
		hostinfo = gethostbyname(hostname);
		strcpy(ipaddr, inet_ntoa(*reinterpret_cast<struct in_addr*>(hostinfo->h_addr_list[0])));
	}
	WSACleanup();
	printf("*/GAME SERVER/*\n");
	printf("This Server's IP address : %s\n", ipaddr);
}

void IOCP_CORE::IOCP_Initialize()
{

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (g_hIocp == NULL) {
		int err_no = WSAGetLastError();
		IOCP_ErrorQuit(L"IOCP_CORE::IOCP_Initialize", err_no);
	}
}

void IOCP_CORE::CheckThisCPUcoreCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	cpuCore = static_cast<int>(si.dwNumberOfProcessors);
	printf("CPU Core Count = %d, threads = %d\n", cpuCore, cpuCore*2);


}

void IOCP_CORE::IOCP_MakeWorkerThreads()
{
	worker_threads.reserve(cpuCore);

	for (int i = 0; i < cpuCore*2; ++i)
	{
		worker_threads.push_back(new thread{ &IOCP_CORE::IOCP_WorkerThread, this });
	}

	thread acceptThread{ &IOCP_CORE::IOCP_AcceptThread, this };
	while (ServerShutdown) { Sleep(1000); }

	for (auto thread : worker_threads) {
		thread->join();
		delete thread;
	}

	acceptThread.join();

	timer_thread.join();
}

void IOCP_CORE::IOCP_WorkerThread() {
	while (TRUE == (!ServerShutdown)) {
		DWORD key;
		DWORD iosize;
		OVLP_EX* my_overlap;

		BOOL result = GetQueuedCompletionStatus(g_hIocp, &iosize, (PULONG_PTR)&key, reinterpret_cast<LPOVERLAPPED*>(&my_overlap), INFINITE);

		auto client_it = g_players.find(key);
		if (client_it == g_players.end()) {
			// 유효하지 않은 클라이언트 키에 대한 처리, 예: 로그 출력, 계속하기
			continue;
		}
		PLAYER_INFO* user = client_it->second;

		// 클라이언트가 접속을 끊었을 경우
		if (FALSE == result || 0 == iosize) {
			if (FALSE == result) {
				int err_no = WSAGetLastError();
				IOCP_ErrorDisplay("WorkerThreadStart::GetQueuedCompletionStatus", err_no, __LINE__);
			}

			closesocket(user->s);
			user->connected = false;
			DisconnectClient(user->id);
			printf("[ No. %3u ] Disconnected\n", key);

			continue;
		}
		else if (OP_SERVER_RECV == my_overlap->operation) {
			// 클라이언트로부터 데이터를 받았을 경우
			char* buf_ptr = reinterpret_cast<char*>(user->recv_overlap.iocp_buffer);
			int remained = iosize;

			// 남은 데이터를 처리하는 로직
			while (0 < remained) {
				// 버퍼에 데이터를 추가
				int copySize = min(remained, MAX_PACKET_SIZE - user->previous_size);
				memcpy(user->packet_buff + user->previous_size, buf_ptr, copySize);
				user->previous_size += copySize;
				buf_ptr += copySize;
				remained -= copySize;

				if (IOCP_CORE::IOCP_ProcessPacket(key, user->packet_buff, user->previous_size)) {
					// 파싱 및 처리가 성공적으로 완료되면 버퍼 초기화
					user->previous_size = 0;
				}
			}

			// 다음 데이터를 받기 위한 WSARecv 호출
			DWORD flags = 0;
			int retval = WSARecv(user->s, &user->recv_overlap.wsabuf, 1, NULL, &flags, &user->recv_overlap.original_overlap, NULL);
			if (SOCKET_ERROR == retval) {
				int err_no = WSAGetLastError();
				if (ERROR_IO_PENDING != err_no) {
					IOCP_ErrorDisplay("WorkerThreadStart::WSARecv", err_no, __LINE__);
				}
				continue;
			}
		}
		else if (OP_SERVER_SEND == my_overlap->operation) {
			// 서버에서 메세지를 보냈으면, 메모리를 해제해 준다.
			delete my_overlap;
		}
		else {
			cout << "Unknown IOCP event !!\n";
			exit(-1);
		}
	}
}


void IOCP_CORE::IOCP_AcceptThread()
{
	int retval{ 0 };

	// socket() - IPv4 ( AF_INET )
	SOCKET listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listen_sock == INVALID_SOCKET) {
		int err_no = WSAGetLastError();
		IOCP_ErrorQuit(L"socket()", err_no);
	};

	// bind()
	struct sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = ::bind(listen_sock, reinterpret_cast<struct sockaddr *>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		IOCP_ErrorQuit(L"socket()", err_no);
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		IOCP_ErrorQuit(L"socket()", err_no);
	}

	while (TRUE == (!ServerShutdown)) {
		// accept()
		struct sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		SOCKET client_sock = WSAAccept(listen_sock, reinterpret_cast<sockaddr *>(&clientaddr), &addrlen, NULL, NULL);
		if (INVALID_SOCKET == client_sock) {
			int err_no = WSAGetLastError();
			IOCP_ErrorDisplay("Accept::WSAAccept", err_no, __LINE__);
			while (true);
		}

		/* DB 관련 login 기능이 여기에 추가되어야 한다. 로그인이 번호가 제대로 맞으면 통과, 아니면 클라이언트 연결을 끊는다. 로그인을 하면 DB에서 정보를 가져온다 */

		playerIndex += 1;
		printf("[ No. %3u ] Client IP = %s, Port = %d is Connected\n", playerIndex, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		WSABUF wsabuf;
		wsabuf.buf = reinterpret_cast<char*>(&playerIndex);
		wsabuf.len = sizeof(playerIndex);

		DWORD sent;
		OVERLAPPED sendOverlap;
		ZeroMemory(&sendOverlap, sizeof(sendOverlap));

		retval = WSASend(client_sock, &wsabuf, 1, &sent, 0, &sendOverlap, NULL);
		if (retval == SOCKET_ERROR) {
			int err_no = WSAGetLastError();
			IOCP_ErrorQuit(L"id send()", err_no);
		}

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), g_hIocp, playerIndex, 0);

		PLAYER_INFO *user = new PLAYER_INFO;

		user->s = client_sock;
		user->connected = true;
		user->id = playerIndex;
		user->packet_size = 0;
		user->previous_size = 0;
		memset(&user->recv_overlap.original_overlap, 0, sizeof(user->recv_overlap.original_overlap));
		user->recv_overlap.operation = OP_SERVER_RECV;
		user->recv_overlap.wsabuf.buf = reinterpret_cast<char*>(&user->recv_overlap.iocp_buffer);
		user->recv_overlap.wsabuf.len = sizeof(user->recv_overlap.iocp_buffer);
		user->isInGame = false;

		g_players[user->id] = user;

		/* 주변 클라이언트에 대해 뿌릴 정보 뿌리고, 시야 리스트나 처리해야 할 정보들도 함께 넣는다. */

		// 클라이언트에서 응답오길 기다리기
		DWORD flags{ 0 };
		retval = WSARecv(client_sock, &user->recv_overlap.wsabuf, 1, NULL, &flags, &user->recv_overlap.original_overlap, NULL);
		if (SOCKET_ERROR == retval) {
			int err_no = WSAGetLastError();
			if (ERROR_IO_PENDING != err_no) {
				IOCP_ErrorDisplay("Accept::WSARecv", err_no, __LINE__);
			}
		}
	}
}

void IOCP_CORE::DisconnectClient(unsigned int id) {
    if (g_players.find(id) != g_players.end()) {
        PLAYER_INFO *user = g_players[id];

        delete user;
        g_players.erase(id);
    }
}


void IOCP_CORE::IOCP_SendPacket(unsigned int id, const char* serializedData, size_t dataSize)
{
	auto it = g_players.find(id);
	if (it == g_players.end() || !(it->second->connected)) {
		return;
	}

	PLAYER_INFO* user = it->second;

	OVLP_EX* over = new OVLP_EX;
	memset(over, 0, sizeof(OVLP_EX));
	over->operation = OP_SERVER_SEND;
	over->wsabuf.buf = reinterpret_cast<char*>(over->iocp_buffer);
	over->wsabuf.len = static_cast<ULONG>(dataSize); // dataSize를 ULONG으로 변환하여 할당
	memcpy(over->iocp_buffer, serializedData, dataSize); // 직렬화된 데이터를 iocp_buffer에 복사

	DWORD flags{ 0 };
	int retval = WSASend(user->s, &over->wsabuf, 1, NULL, flags, &over->original_overlap, NULL);
	if (SOCKET_ERROR == retval) {
		int err_no = WSAGetLastError();
		if (ERROR_IO_PENDING != err_no) {
			IOCP_ErrorDisplay("SendPacket::WSASend", err_no, __LINE__);
		}
	}
}

void IOCP_CORE::IOCP_ErrorDisplay(const char *msg, int err_no, int line)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[ %s - %d ]", msg, line);
	wprintf(L"에러 %s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void IOCP_CORE::IOCP_ErrorQuit(const wchar_t *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void IOCP_CORE::Timer_Thread()
{
	printf("Timer Thread Started\n");
	while (!ServerShutdown)
	{
		if (b_Timer) 
		{
			Protocol::Time packet;

			std::this_thread::sleep_for(std::chrono::seconds(1)); // 1초마다 타이머
			GameTime++;

			std::string gameTimeStr = std::to_string(GameTime);

			packet.set_timer(GameTime);
			packet.set_packet_type(3);

			std::string serializedData;
			packet.SerializeToString(&serializedData);

			for (auto& playerPair : g_players)
			{
				PLAYER_INFO* player = playerPair.second;
				if (player->connected) {
					IOCP_SendPacket(player->id, serializedData.data(), serializedData.size());
				}
			}

			//printf("Send Timer");
		}
	}
}

void IOCP_CORE::Zombie_BT_Thread()
{
	//플레이어 초기 위치
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{2299.f, 3857.f, 952.f}} };
	//좀비 초기 위치
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{988.f, 2964.f, 952.f}} };

	//플레이어 인스턴스
	Player* p = new Player(pl);
	//좀비 인스턴스
	Zombie* z = new Zombie(p, "zombieee", zl);

	vector<vector<vector<float>>> p_l = p->PlayerLocation;
	float p_x = p_l[0][0][0]; float p_y = p_l[0][0][1]; float p_z = p_l[0][0][2];
	float z_x = z->ZombieLocation[0][0][0]; float z_y = z->ZombieLocation[0][0][1]; float z_z = z->ZombieLocation[0][0][2];
	cout << "플레이어의 시작 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
	cout << "좀비의 시작 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
	cout << endl;


	//======[좀비 BT 생성]======

	//==========선언============

	//<Selector> 선언 

	//<Selector-Detect> (사실상 최상위 노드)
	Selector sel_detect;
	//<Selector-CanSeePlayer>  
	Selector sel_canseeplayer;

	//{Sequence} 선언

	//{Sequence-CanNotAttack}
	Sequence seq_cannotattack;
	//{Sequence-CanAttack}
	Sequence seq_canattack;
	//
	//{Sequence-HasInvestigated}
	Sequence seq_hasinvestigated;
	//{Sequence-NotHasLastKnownPlayerLocation}
	Sequence seq_nothaslastknownplayerlocation;


	//[Task] 선언 & 메모리 할당

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer = new TCanSeePlayer;
	//[HasInvestigated-Task]
	THasInvestigated* t_hasinvestigated = new THasInvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;

	//<Selector CanSeePlayer> 가 가지는 Task들

	//[CanNotAttack-Task]
	TCanNotAttack* t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	TCanAttack* t_canattack = new TCanAttack;

	//{Sequence} 가 가지는 Task들

	//[MoveTo-Task]
	TMoveTo* t_moveto = new TMoveTo;
	//[Attack-Task]
	TAttack* t_attack = new TAttack;


	//========작업 할당==========

	//<Selector-Detect> 할당
	//<Selector-Detect>에 해당 Task들 '순서대로' 삽입
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> 할당
	//<Selector-CanSeePlayer>에 해당 Task들 '순서대로' 삽입
	sel_canseeplayer.AddChild(t_canattack);
	sel_canseeplayer.AddChild(t_cannotattack);

	//{Sequence-CanNotAttack} 할당
	//{Sequence-CanNotAttack}에 해당 Task들 '순서대로' 삽입
	seq_cannotattack.AddChild(t_moveto);

	//{Sequence-CanAttack} 할당
	//{Sequence-CanAttack}에 해당 Task들 '순서대로' 삽입
	seq_canattack.AddChild(t_attack);

	//{Sequence-HasInvestigated} 할당
	//{Sequence-HasInvestigated}에 해당 Task들 '순서대로' 삽입
	seq_hasinvestigated.AddChild(t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} 할당
	//{Sequence-NotHasLastKnownPlayerLocation}에 해당 Task들 '순서대로' 삽입
	seq_nothaslastknownplayerlocation.AddChild(t_moveto);


	//========작업 실행==========

	string result = "Initial";

	while (true) {

		cout << "========BT 실행==========" << endl;
		cout << endl;;

		//좀비와 플레이어의 거리 갱신
		z->SetDistance();

		//<Selector-Detect> 실행
		result = sel_detect.Sel_Detect(*z);

		//<Selector-Detect> 결과 값에 따라 다음 Task들 실행
		if (result == "CanSeePlayer-Succeed") {

			//<Selector-CanSeePlayer> 실행
			result = sel_canseeplayer.Sel_CanSeePlayer(*z);

			//<Selector-CanSeePlayer> 결과 값에 따라 다음 Task들 실행
			if (result == "CanAttack-Succeed") {

				//{Sequence-CanAttack} 실행
				result = seq_canattack.Seq_CanAttack(*z);

			}
			else if (result == "CanNotAttack-Succeed") {

				//{Sequence-CanNotAttack} 실행
				result = seq_cannotattack.Seq_CanNotAttack(*z);

			}
			else {	//result == "Fail"
				cout << "EEEERRRROOOOOORRRR" << endl;
			}

		}
		else if (result == "HasInvestigated-Succeed") {

			//{Sequence-HasInvestigated} 실행
			result = seq_hasinvestigated.Seq_HasInvestigated(*z);

		}
		else if (result == "NotHasLastKnownPlayerLocation-Succeed") {

			//{Sequence-NotHasLastKnownPlayerLocation} 실행
			result = seq_nothaslastknownplayerlocation.Seq_NotHasLastKnownPlayerLocation(*z);

		}
		else {	//result == "Fail"
			cout << "EEEERRRROOOOOORRRR" << endl;
		}

		cout << endl;
		float p_x = p->PlayerLocation[0][0][0]; float p_y = p->PlayerLocation[0][0][1]; float p_z = p->PlayerLocation[0][0][2];
		float z_x = z->ZombieLocation[0][0][0]; float z_y = z->ZombieLocation[0][0][1]; float z_z = z->ZombieLocation[0][0][2];
		cout << "플레이어의 현제 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
		cout << "좀비의 현제 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
		cout << endl;

		//플레이어 입력 받기
		//bool proper_input = false;
		//int input_x = 0;	// 6:x++, 4:x--
		//int input_y = 0;	// 8:y++, 2:y--
		//while (proper_input == false) {
		//	//cout << "*** 플레이어 이동=> 5: 정지, 6: x++ , 4: x-- , 8: y++ , 2: y--" << endl;
		//	cout << "*** 플레이어 위치 설정" << endl;
		//	//cout << "x축 이동 입력: ";
		//	cout << "x좌표 입력: ";
		//	cin >> input_x;
		//	if (cin.fail()) {
		//		cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
		//		cout << endl;
		//		cin.clear();
		//		cin.ignore(1000, '\n');
		//		continue;
		//	}
		//	cin.clear();
		//	cin.ignore(1000, '\n');
		//	//cout << "y축 이동 입력: ";
		//	cout << "y좌표 입력: ";
		//	cin >> input_y;
		//	if (cin.fail()) {
		//		cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
		//		cout << endl;
		//		cin.clear();
		//		cin.ignore(1000, '\n');
		//		continue;
		//	}
		//	cin.clear();
		//	cin.ignore(1000, '\n');
		//	cout << endl;
		//
			//cout << "플레이어 ";
			//if ((input_x == 5 || input_x == 6 || input_x == 4) && (input_y == 5 || input_y == 8 || input_y == 2)) {
			//	proper_input = true;
			//
			//	if (input_x == 5) {
			//		cout << "x축으로 0 , ";
			//	}
			//	else if (input_x == 6) {
			//		cout << "x축으로 +1 , ";
			//		p->Move(1, 0, 0);
			//	}
			//	else if (input_x == 4) {
			//		cout << "x축으로 -1 , ";
			//		p->Move(-1, 0, 0);
			//	}
			//	else
			//		cout << "[Error]!!!";
			//
			//	if (input_y == 5) {
			//		cout << "y축으로 0 ";
			//	}
			//	else if (input_y == 8) {
			//		cout << "y축으로 +1 ";
			//		p->Move(0, 1, 0);
			//	}
			//	else if (input_y == 2) {
			//		cout << "y축으로 -1 ";
			//		p->Move(0, -1, 0);
			//	}
			//	else
			//		cout << "[Error]!!!";
			//
			//	cout << "이동!!!" << endl;
			//	cout << endl;
			//}
			//else {
			//	proper_input = false;
			//
			//	cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
			//	cout << endl;
			//}
		//
		//	p->Teleport(input_x, input_y, 00000);
		//	break;
		//
		//}

		//콘솔창에서 한 싸이클씩 돌아가게
		cout << "(계속 진행)아무거나 입력 후 엔터: ";
		int one_cycle;
		cin >> one_cycle;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cin.clear();			//한번더 써서 남은 개행 문자까지 싹 지워줌
		cin.ignore(1000, '\n');
		cout << endl;

		p_x = p->PlayerLocation[0][0][0]; p_y = p->PlayerLocation[0][0][1]; p_z = p->PlayerLocation[0][0][2];
		cout << "플레이어의 이전 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
		cout << "좀비의 이전 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
		cout << endl;
	}

	//==========================
}
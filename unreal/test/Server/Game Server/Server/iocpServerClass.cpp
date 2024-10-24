#include <iomanip>

#include "iocpServerClass.h"

#include "ZombiePathfinder.h"

#include "MoveTo.h"
#include "CanAttack.h"
#include "CanNotAttack.h"

std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
std::unordered_map<int, Player> playerDB;


std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapB2;
std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapB1;
std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapF1;
std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapF2;

std::vector<tuple<float, float, float>> g_valispositionsB2;
std::vector<tuple<float, float, float>> g_valispositionsB1;
std::vector<tuple<float, float, float>> g_valispositionsF1;
std::vector<tuple<float, float, float>> g_valispositionsF2;


float IOCP_CORE::BT_INTERVAL = 0.1f;	// BT 작동 인터벌 설정

std::chrono::duration<float> IOCP_CORE::BT_deltaTime;	// MoveTo에서 계산용으로 사용

IOCP_CORE::IOCP_CORE()
{	
	playerIndex = 0;

	bServerOn = false;

	string filePath;

	filePath = "../../../../Project/EdgesB2.txt";
	LoadEdgesMap(filePath, g_valispositionsB2, g_EdgesMapB2);

	filePath = "../../../../Project/EdgesB1.txt";
	LoadEdgesMap(filePath, g_valispositionsB1, g_EdgesMapB1);

	filePath = "../../../../Project/EdgesF1.txt";
	LoadEdgesMap(filePath, g_valispositionsF1, g_EdgesMapF1);

	filePath = "../../../../Project/EdgesF2.txt";
	LoadEdgesMap(filePath, g_valispositionsF2, g_EdgesMapF2);
	
	timer_thread = thread(&IOCP_CORE::Timer_Thread, this);
	
	//==========Zombie_BT 초기화
	Zombie_BT_Initialize();
	//==========Zombie_BT 쓰레드 시작 (Zombie BT 실행 시작)
	zombie_thread = thread(&IOCP_CORE::Zombie_BT_Thread, this);

	//Nodes = nodeclass->LoadNodesFromFile();

	//======좀비 스폰 & 초기화======
	zombieclass = new ZombieController(*this);

	itemclass = new ItemController(*this);

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

	ServerOn();

	thread acceptThread{ &IOCP_CORE::IOCP_AcceptThread, this };
	while (ServerShutdown) { Sleep(1000); }

	for (auto thread : worker_threads) {
		thread->join();
		delete thread;
	}

	acceptThread.join();

	zombie_thread.join();

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

			if (user->connected) {
				closesocket(user->s);
				user->connected = false;
				DisconnectClient(user->id);
				printf("[ No. %3u ] Disconnected\n", key);
			}

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

			IOCP_SendNextPacket(user);
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

		playerIndex += 1;
		printf("\n[ No. %3u ] Client IP = %s, Port = %d is Connected\n", playerIndex, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

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
	{
		std::lock_guard<std::mutex> lock(user->sendMutex);
		user->sendQueue.push(std::string(serializedData, dataSize));

		if (user->isSending) {
			// 이미 전송 중이라면 대기열에만 추가
			return;
		}

		// 현재 전송 중이 아니면 전송 시작
		user->isSending = true;
	}

	// 대기열에서 첫 번째 데이터를 꺼내 전송 시작
	IOCP_SendNextPacket(user);
}

void IOCP_CORE::IOCP_SendNextPacket(PLAYER_INFO* user)
{
	std::lock_guard<std::mutex> lock(user->sendMutex);

	if (user->sendQueue.empty()) {
		user->isSending = false; 
		return;
	}

	std::string& data = user->sendQueue.front();

	OVLP_EX* over = new OVLP_EX;
	memset(over, 0, sizeof(OVLP_EX));
	over->operation = OP_SERVER_SEND;
	over->wsabuf.buf = reinterpret_cast<char*>(over->iocp_buffer);
	over->wsabuf.len = static_cast<ULONG>(data.size());
	memcpy(over->iocp_buffer, data.data(), data.size());

	DWORD flags{ 0 };
	int retval = WSASend(user->s, &over->wsabuf, 1, NULL, flags, &over->original_overlap, NULL);
	if (SOCKET_ERROR == retval) {
		int err_no = WSAGetLastError();
		if (ERROR_IO_PENDING != err_no) {
			IOCP_ErrorDisplay("SendPacket::WSASend", err_no, __LINE__);
		}
	}
	else {
		// 전송 작업이 성공적으로 시작되었으므로, 대기열에서 제거
		user->sendQueue.pop();
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
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void IOCP_CORE::Timer_Thread()
{
	printf("Timer Thread Started\n");
	auto lastTime = std::chrono::high_resolution_clock::now();
	auto lastSendTime = std::chrono::high_resolution_clock::now();
	auto lastPingTime = std::chrono::high_resolution_clock::now();

	while (!ServerShutdown)
	{
		if (b_Timer) 
		{
			// 현재 시간 측정
			auto currentTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			// deltaTime을 누적하여 GameTime에 더함
			GameTime += deltaTime.count();  // 초 단위
			
			//for (auto& zom : zombieDB) {
			//	if (!(zom->path.empty())) {
					//if (zom->ZombiePathIndex >= zom->path.size()) {
					//	 // cout << "Zombie has reached the final destination." << endl;
					//	continue; // 경로 끝에 도달
					//}
					//zom->MoveTo(deltaTime.count());
			//	}
			//}

			// GameTime을 문자열로 변환
			//std::string gameTimeStr = std::to_string(GameTime);



			std::chrono::duration<float> sendInterval = currentTime - lastSendTime;
			if (sendInterval.count() >= 1.0f) {
				Protocol::Time packet;
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
					//cout << "send" << '\n';
					//printf("Send Timer");
				}

				lastSendTime = currentTime;
			}

			// 5초마다 Ping 메시지 전송
			std::chrono::duration<float> pingInterval = currentTime - lastPingTime;
			if (pingInterval.count() >= 5.0f) {
				SendPingToClients();
				lastPingTime = currentTime;
			}
		}
	}
}

void IOCP_CORE::Zombie_BT_Initialize()
{
	//======[좀비 BT 생성]======
	//======메모리 할당 -> 작업 할당======

	//======[Task] 메모리 할당======

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	t_canseeplayer = new TCanSeePlayer;
	//[HasShouting-Task]
	t_hasshouting = new THasShouting;
	//[HasFootSound-Task]
	t_hasfootsound = new THasFootSound;
	//[HasInvestigated-Task]
	t_hasinvestigated = new THasInvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;

	//<Selector CanSeePlayer> 가 가지는 Task들

	//[CanNotAttack-Task]
	t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	t_canattack = new TCanAttack;

	//{Sequence} 가 가지는 Task들

	//[MoveTo-Task]
	t_moveto = new TMoveTo;
	//[Attack-Task]
	t_attack = new TAttack;

	//========작업 할당========

	//<Selector-Detect> 할당
	//<Selector-Detect>에 해당 Task들 '순서대로' 삽입
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasshouting);
	sel_detect.AddChild(t_hasfootsound);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> 할당
	//<Selector-CanSeePlayer>에 해당 Task들 '순서대로' 삽입
	sel_canseeplayer.AddChild(t_canattack);
	sel_canseeplayer.AddChild(t_cannotattack);

	//{Sequence-CanAttack} 할당
	//{Sequence-CanAttack}에 해당 Task들 '순서대로' 삽입
	seq_canattack.AddChild(t_attack);

	//{Sequence-CanNotAttack} 할당
	//{Sequence-CanNotAttack}에 해당 Task들 '순서대로' 삽입
	seq_cannotattack.AddChild(t_moveto);

	//{Sequence-HasShouting} 할당
	//{Sequence-HasShouting}에 해당 Task들 '순서대로' 삽입
	seq_hasshouting.AddChild(t_moveto);

	//{Sequence-HasFootSound} 할당
	//{Sequence-HasFootSound}에 해당 Task들 '순서대로' 삽입
	seq_hasfootsound.AddChild(t_moveto);

	//{Sequence-HasInvestigated} 할당
	//{Sequence-HasInvestigated}에 해당 Task들 '순서대로' 삽입
	seq_hasinvestigated.AddChild(t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} 할당
	//{Sequence-NotHasLastKnownPlayerLocation}에 해당 Task들 '순서대로' 삽입
	seq_nothaslastknownplayerlocation.AddChild(t_moveto);

	//==========================
}

void IOCP_CORE::ServerOn()
{
	cout << endl;

	cout << std::setfill(' ') << std::showpoint << std::fixed << std::setprecision(2);		// 출력 칸 맞추기 (소수점 2자리까지)

	for (const auto player : playerDB) {
		float p_x = player.second.x;
		float p_y = player.second.y;
		float p_z = player.second.z;

		cout << "플레이어 \'#" << player.first << "\' 의 시작 위치: ( "
			<< std::setw(8) << p_x << ", " << std::setw(8) << p_y << ", " << std::setw(8) << p_z << " )" << endl;
	}

	cout << endl;

	for (const auto zom : zombieDB) {
		float z_x = zom->ZombieData.x;
		float z_y = zom->ZombieData.y;
		float z_z = zom->ZombieData.z;

		cout << "좀비 \'#" << zom->ZombieData.zombieID << "\' 의 시작 위치: ( "
			<< std::setw(8) << z_x << ", " << std::setw(8) << z_y << ", " << std::setw(8) << z_z << " ) , HP: " << zom->GetHP() << endl;
	}

	cout << endl;

	bServerOn = true;
}

void IOCP_CORE::Zombie_BT_Thread()
{
	//========작업 실행==========

	string result = "Initial";

	auto lastBTTime = std::chrono::high_resolution_clock::now();
	
	while (true) {

		//서버가 먼저 켜지고 좀비 BT가 실행되도록
		if (bServerOn == false)
			continue;
		
		auto currentTime = std::chrono::high_resolution_clock::now();

		BT_deltaTime = currentTime - lastBTTime;
		if (BT_deltaTime.count() < BT_INTERVAL) {
			continue;							
		}

		//cout << endl;

		lastBTTime = currentTime;


		if (playerDB.size() == 0) {
			//cout << "연결된 플레이어가 없습니다..." << endl;
			//cout << endl;
			result = "NO PLAYER";
		}
		else {
			//for (auto player : playerDB) {
			//	float p_x = player.second.x;					float p_y = player.second.y;					float p_z = player.second.z;
			//	cout << "플레이어 \'#" << player.first << "\' 의 현재 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
			//	//cout << endl;
			//}
			//cout << endl;

			result = "HAS PLAYER";
		}
		

		// 좀비가 있을때 BT 실행
		for (auto& zom : zombieDB) {

			// 플레이어가 있을때 BT 실행
			if (result == "NO PLAYER")
				break;

			// 좀비가 사망시 BT 중지
			if (zom->zombieHP <= 0.f) {
				continue;
			}

			// 좀비가 대기상태라면 해당 좀비 BT 잠시 대기
			if (zom->HaveToWait == true) {
				zom->Wait();
				continue;
			}

			//시작하자 말자 플레이어 위치로 움직이게 설정 (A* 관찰용)
			//zom->PlayerInSight = true;

			//cout << endl;
			//cout << "========좀비 \'#" << zom->ZombieData.zombieID << "\' BT 실행==========" << endl;
			//cout << endl;
			//
			//float z_x = zom->ZombieData.x;					float z_y = zom->ZombieData.y;					float z_z = zom->ZombieData.z;
			//cout << "좀비 \'#" << zom->ZombieData.zombieID << "\' 의 현재 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
			//cout << endl;


			//<Selector-Detect> 실행
			result = sel_detect.Sel_Detect(*zom);

			//<Selector-Detect> 결과 값에 따라 다음 Task들 실행
			if (result == "CanSeePlayer-Succeed") {

				//<Selector-CanSeePlayer> 실행
				result = sel_canseeplayer.Sel_CanSeePlayer(*zom);

				//<Selector-CanSeePlayer> 결과 값에 따라 다음 Task들 실행
				if (result == "CanAttack-Succeed") {

					//{Sequence-CanAttack} 실행
					result = seq_canattack.Seq_CanAttack(*zom);

				}
				else if (result == "CanNotAttack-Succeed") {

					//{Sequence-CanNotAttack} 실행
					result = seq_cannotattack.Seq_CanNotAttack(*zom);

				}
				else {	//result == "Fail"
					cout << "<Selector-CanSeePlayer> EEEERRRROOOOOORRRR - ZombieID #" << zom->ZombieData.zombieID << endl;
				}

			}
			else if (result == "HasShouting-Succeed") {

				//{Sequence-HasShouting} 실행
				result = seq_hasshouting.Seq_HasShouting(*zom);

			}
			else if (result == "HasFootSound-Succeed") {

				//{Sequence-HasFootSound} 실행
				result = seq_hasfootsound.Seq_HasFootSound(*zom);

			}
			else if (result == "HasInvestigated-Succeed") {

				//{Sequence-HasInvestigated} 실행
				result = seq_hasinvestigated.Seq_HasInvestigated(*zom);

			}
			else if (result == "NotHasLastKnownPlayerLocation-Succeed") {

				//{Sequence-NotHasLastKnownPlayerLocation} 실행
				result = seq_nothaslastknownplayerlocation.Seq_NotHasLastKnownPlayerLocation(*zom);

			}
			else {	//result == "Fail"
				cout << "<Selector-Detect> EEEERRRROOOOOORRRR - ZombieID #" << zom->ZombieData.zombieID << endl;
			}

			
			//z_x = zom->ZombieData.x;					z_y = zom->ZombieData.y;					z_z = zom->ZombieData.z;
			//cout << "좀비 \'#" << zom->ZombieData.zombieID << "\' 의 새로운 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
			//cout << endl;


		//cout << "========좀비 \'#" << zom->ZombieData.zombieID << "\' BT 종료==========" << endl;
		//cout << endl;
		}


		//콘솔창에서 한 싸이클씩 돌아가게
		//cout << "(계속 진행)아무거나 입력 후 엔터: ";
		//char one_cycle;
		//cin >> one_cycle;
		//if (cin.fail()) {
		//	cin.clear();
		//	cin.ignore(1000, '\n');
		//}
		//cin.clear();			//한번더 써서 남은 개행 문자까지 싹 지워줌
		//cin.ignore(1000, '\n');
		//cout << endl;

	}

	//==========================


	//========할당한 메모리 해제========

	delete(t_canseeplayer);
	delete(t_cannotattack);
	delete(t_moveto);
	delete(t_canattack);
	delete(t_attack);
	delete(t_hasinvestigated);
	delete(t_nothaslastknownplayerlocation);
}







//===================================================================================================================
// 전처리문 나중에 위로 올리기

#include <fstream>
#include <sstream>
#include <string>

bool IOCP_CORE::LoadEdgesMap(const string& filePath, vector<tuple<float, float, float>>& positions, unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash>& EdgesMap)
{
	 ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filePath << endl;
        return false;
    }

    string line;
    tuple<float, float, float> currentNode;
    tuple<float, float, float> neighborNode;
    while (getline(file, line)) {
        stringstream ss(line);

        // "Node:" 줄 처리
        if (line.find("Node:") != string::npos) {
            // Node 정보 파싱
            float x, y, z;
            char comma;
            ss.ignore(5); // "Node: " 무시
            if (ss >> x >> comma >> y >> comma >> z) {
                currentNode = make_tuple(x, y, z);
                EdgesMap[currentNode] = {};  // 새로운 노드 추가

                // validPositions에 currentNode 추가
                positions.push_back(currentNode);
            }
        }
        // "Neighbor:" 줄 처리
        else if (line.find("Neighbor:") != string::npos) {
            // Neighbor 정보 파싱
            float nx, ny, nz;
            char comma;
            ss.ignore(9); // "Neighbor: " 무시

            // x, y, z 좌표 파싱
            if (ss >> nx >> comma >> ny >> comma >> nz) {
                neighborNode = make_tuple(nx, ny, nz);
            }

            // 다음 줄에서 "Weight:" 정보를 가져오기
            if (getline(file, line)) {
                stringstream weightStream(line);
                string temp;
                float weight;

                // "Weight: " 무시하고 가중치 값 추출
                if (weightStream >> temp >> weight && temp == "Weight:") {
                    // 이웃 노드와 가중치 추가
                    EdgesMap[currentNode].emplace_back(neighborNode, weight);
                }
            }
        }
    }

    file.close();
    return true;
}

void IOCP_CORE::SendPingToClients()
{
	Protocol::ping pingpacket;

	pingpacket.set_packet_type(11);

	std::string serializedData;
	pingpacket.SerializeToString(&serializedData);

	for (auto it = g_players.begin(); it != g_players.end(); )
	{
		PLAYER_INFO* player = it->second;

		if (player->connected)
		{
			IOCP_SendPacket(player->id, serializedData.data(), serializedData.size());
			player->pingcnt++;

			if (player->pingcnt >= 3)
			{
				printf("Client #%u did not respond\n", player->id);

				// 클라이언트 연결 해제
				player->connected = false;

				// 클라이언트 삭제 후, 다음 반복자를 얻음
				it = g_players.erase(it);  // erase()는 삭제된 요소의 다음 반복자를 반환
				DisconnectClient(player->id);
				continue;  // 삭제한 후, 다음 클라이언트로 이동
			}
		}
		++it;  // 삭제되지 않은 경우에만 반복자를 증가시킴
	}
}
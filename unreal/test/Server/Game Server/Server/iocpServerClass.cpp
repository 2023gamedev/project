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
			// ��ȿ���� ���� Ŭ���̾�Ʈ Ű�� ���� ó��, ��: �α� ���, ����ϱ�
			continue;
		}
		PLAYER_INFO* user = client_it->second;

		// Ŭ���̾�Ʈ�� ������ ������ ���
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
			// Ŭ���̾�Ʈ�κ��� �����͸� �޾��� ���
			char* buf_ptr = reinterpret_cast<char*>(user->recv_overlap.iocp_buffer);
			int remained = iosize;

			// ���� �����͸� ó���ϴ� ����
			while (0 < remained) {
				// ���ۿ� �����͸� �߰�
				int copySize = min(remained, MAX_PACKET_SIZE - user->previous_size);
				memcpy(user->packet_buff + user->previous_size, buf_ptr, copySize);
				user->previous_size += copySize;
				buf_ptr += copySize;
				remained -= copySize;

				if (IOCP_CORE::IOCP_ProcessPacket(key, user->packet_buff, user->previous_size)) {
					// �Ľ� �� ó���� ���������� �Ϸ�Ǹ� ���� �ʱ�ȭ
					user->previous_size = 0;
				}
			}

			// ���� �����͸� �ޱ� ���� WSARecv ȣ��
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
			// �������� �޼����� ��������, �޸𸮸� ������ �ش�.
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

		/* DB ���� login ����� ���⿡ �߰��Ǿ�� �Ѵ�. �α����� ��ȣ�� ����� ������ ���, �ƴϸ� Ŭ���̾�Ʈ ������ ���´�. �α����� �ϸ� DB���� ������ �����´� */

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

		/* �ֺ� Ŭ���̾�Ʈ�� ���� �Ѹ� ���� �Ѹ���, �þ� ����Ʈ�� ó���ؾ� �� �����鵵 �Բ� �ִ´�. */

		// Ŭ���̾�Ʈ���� ������� ��ٸ���
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
	over->wsabuf.len = static_cast<ULONG>(dataSize); // dataSize�� ULONG���� ��ȯ�Ͽ� �Ҵ�
	memcpy(over->iocp_buffer, serializedData, dataSize); // ����ȭ�� �����͸� iocp_buffer�� ����

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
	wprintf(L"���� %s\n", lpMsgBuf);
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

			std::this_thread::sleep_for(std::chrono::seconds(1)); // 1�ʸ��� Ÿ�̸�
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
	//�÷��̾� �ʱ� ��ġ
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{2299.f, 3857.f, 952.f}} };
	//���� �ʱ� ��ġ
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{988.f, 2964.f, 952.f}} };

	//�÷��̾� �ν��Ͻ�
	Player* p = new Player(pl);
	//���� �ν��Ͻ�
	Zombie* z = new Zombie(p, "zombieee", zl);

	vector<vector<vector<float>>> p_l = p->PlayerLocation;
	float p_x = p_l[0][0][0]; float p_y = p_l[0][0][1]; float p_z = p_l[0][0][2];
	float z_x = z->ZombieLocation[0][0][0]; float z_y = z->ZombieLocation[0][0][1]; float z_z = z->ZombieLocation[0][0][2];
	cout << "�÷��̾��� ���� ��ġ: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
	cout << "������ ���� ��ġ: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
	cout << endl;


	//======[���� BT ����]======

	//==========����============

	//<Selector> ���� 

	//<Selector-Detect> (��ǻ� �ֻ��� ���)
	Selector sel_detect;
	//<Selector-CanSeePlayer>  
	Selector sel_canseeplayer;

	//{Sequence} ����

	//{Sequence-CanNotAttack}
	Sequence seq_cannotattack;
	//{Sequence-CanAttack}
	Sequence seq_canattack;
	//
	//{Sequence-HasInvestigated}
	Sequence seq_hasinvestigated;
	//{Sequence-NotHasLastKnownPlayerLocation}
	Sequence seq_nothaslastknownplayerlocation;


	//[Task] ���� & �޸� �Ҵ�

	//<Selector Detact> �� ������ Task��

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer = new TCanSeePlayer;
	//[HasInvestigated-Task]
	THasInvestigated* t_hasinvestigated = new THasInvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;

	//<Selector CanSeePlayer> �� ������ Task��

	//[CanNotAttack-Task]
	TCanNotAttack* t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	TCanAttack* t_canattack = new TCanAttack;

	//{Sequence} �� ������ Task��

	//[MoveTo-Task]
	TMoveTo* t_moveto = new TMoveTo;
	//[Attack-Task]
	TAttack* t_attack = new TAttack;


	//========�۾� �Ҵ�==========

	//<Selector-Detect> �Ҵ�
	//<Selector-Detect>�� �ش� Task�� '�������' ����
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> �Ҵ�
	//<Selector-CanSeePlayer>�� �ش� Task�� '�������' ����
	sel_canseeplayer.AddChild(t_canattack);
	sel_canseeplayer.AddChild(t_cannotattack);

	//{Sequence-CanNotAttack} �Ҵ�
	//{Sequence-CanNotAttack}�� �ش� Task�� '�������' ����
	seq_cannotattack.AddChild(t_moveto);

	//{Sequence-CanAttack} �Ҵ�
	//{Sequence-CanAttack}�� �ش� Task�� '�������' ����
	seq_canattack.AddChild(t_attack);

	//{Sequence-HasInvestigated} �Ҵ�
	//{Sequence-HasInvestigated}�� �ش� Task�� '�������' ����
	seq_hasinvestigated.AddChild(t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} �Ҵ�
	//{Sequence-NotHasLastKnownPlayerLocation}�� �ش� Task�� '�������' ����
	seq_nothaslastknownplayerlocation.AddChild(t_moveto);


	//========�۾� ����==========

	string result = "Initial";

	while (true) {

		cout << "========BT ����==========" << endl;
		cout << endl;;

		//����� �÷��̾��� �Ÿ� ����
		z->SetDistance();

		//<Selector-Detect> ����
		result = sel_detect.Sel_Detect(*z);

		//<Selector-Detect> ��� ���� ���� ���� Task�� ����
		if (result == "CanSeePlayer-Succeed") {

			//<Selector-CanSeePlayer> ����
			result = sel_canseeplayer.Sel_CanSeePlayer(*z);

			//<Selector-CanSeePlayer> ��� ���� ���� ���� Task�� ����
			if (result == "CanAttack-Succeed") {

				//{Sequence-CanAttack} ����
				result = seq_canattack.Seq_CanAttack(*z);

			}
			else if (result == "CanNotAttack-Succeed") {

				//{Sequence-CanNotAttack} ����
				result = seq_cannotattack.Seq_CanNotAttack(*z);

			}
			else {	//result == "Fail"
				cout << "EEEERRRROOOOOORRRR" << endl;
			}

		}
		else if (result == "HasInvestigated-Succeed") {

			//{Sequence-HasInvestigated} ����
			result = seq_hasinvestigated.Seq_HasInvestigated(*z);

		}
		else if (result == "NotHasLastKnownPlayerLocation-Succeed") {

			//{Sequence-NotHasLastKnownPlayerLocation} ����
			result = seq_nothaslastknownplayerlocation.Seq_NotHasLastKnownPlayerLocation(*z);

		}
		else {	//result == "Fail"
			cout << "EEEERRRROOOOOORRRR" << endl;
		}

		cout << endl;
		float p_x = p->PlayerLocation[0][0][0]; float p_y = p->PlayerLocation[0][0][1]; float p_z = p->PlayerLocation[0][0][2];
		float z_x = z->ZombieLocation[0][0][0]; float z_y = z->ZombieLocation[0][0][1]; float z_z = z->ZombieLocation[0][0][2];
		cout << "�÷��̾��� ���� ��ġ: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
		cout << "������ ���� ��ġ: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
		cout << endl;

		//�÷��̾� �Է� �ޱ�
		//bool proper_input = false;
		//int input_x = 0;	// 6:x++, 4:x--
		//int input_y = 0;	// 8:y++, 2:y--
		//while (proper_input == false) {
		//	//cout << "*** �÷��̾� �̵�=> 5: ����, 6: x++ , 4: x-- , 8: y++ , 2: y--" << endl;
		//	cout << "*** �÷��̾� ��ġ ����" << endl;
		//	//cout << "x�� �̵� �Է�: ";
		//	cout << "x��ǥ �Է�: ";
		//	cin >> input_x;
		//	if (cin.fail()) {
		//		cout << "�߸��� �Է��Դϴ� �ٽ� �Է��� �ּ���." << endl;
		//		cout << endl;
		//		cin.clear();
		//		cin.ignore(1000, '\n');
		//		continue;
		//	}
		//	cin.clear();
		//	cin.ignore(1000, '\n');
		//	//cout << "y�� �̵� �Է�: ";
		//	cout << "y��ǥ �Է�: ";
		//	cin >> input_y;
		//	if (cin.fail()) {
		//		cout << "�߸��� �Է��Դϴ� �ٽ� �Է��� �ּ���." << endl;
		//		cout << endl;
		//		cin.clear();
		//		cin.ignore(1000, '\n');
		//		continue;
		//	}
		//	cin.clear();
		//	cin.ignore(1000, '\n');
		//	cout << endl;
		//
			//cout << "�÷��̾� ";
			//if ((input_x == 5 || input_x == 6 || input_x == 4) && (input_y == 5 || input_y == 8 || input_y == 2)) {
			//	proper_input = true;
			//
			//	if (input_x == 5) {
			//		cout << "x������ 0 , ";
			//	}
			//	else if (input_x == 6) {
			//		cout << "x������ +1 , ";
			//		p->Move(1, 0, 0);
			//	}
			//	else if (input_x == 4) {
			//		cout << "x������ -1 , ";
			//		p->Move(-1, 0, 0);
			//	}
			//	else
			//		cout << "[Error]!!!";
			//
			//	if (input_y == 5) {
			//		cout << "y������ 0 ";
			//	}
			//	else if (input_y == 8) {
			//		cout << "y������ +1 ";
			//		p->Move(0, 1, 0);
			//	}
			//	else if (input_y == 2) {
			//		cout << "y������ -1 ";
			//		p->Move(0, -1, 0);
			//	}
			//	else
			//		cout << "[Error]!!!";
			//
			//	cout << "�̵�!!!" << endl;
			//	cout << endl;
			//}
			//else {
			//	proper_input = false;
			//
			//	cout << "�߸��� �Է��Դϴ� �ٽ� �Է��� �ּ���." << endl;
			//	cout << endl;
			//}
		//
		//	p->Teleport(input_x, input_y, 00000);
		//	break;
		//
		//}

		//�ܼ�â���� �� ����Ŭ�� ���ư���
		cout << "(��� ����)�ƹ��ų� �Է� �� ����: ";
		int one_cycle;
		cin >> one_cycle;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
		}
		cin.clear();			//�ѹ��� �Ἥ ���� ���� ���ڱ��� �� ������
		cin.ignore(1000, '\n');
		cout << endl;

		p_x = p->PlayerLocation[0][0][0]; p_y = p->PlayerLocation[0][0][1]; p_z = p->PlayerLocation[0][0][2];
		cout << "�÷��̾��� ���� ��ġ: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
		cout << "������ ���� ��ġ: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
		cout << endl;
	}

	//==========================
}
#pragma once
#include"Common.h"

std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
std::unordered_map<unsigned int, bool> players_ready;
std::unordered_map<int, Room*> rooms;
std::mutex g_players_mutex;

IOCP_CORE::IOCP_CORE()
{	
	playerIndex = 0;

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
	printf("*/LOBBY SERVER/*\n");
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

	for (int i = 1; i <= 4; ++i) {
		rooms.emplace(i, new Room(i)); // room ID를 key로 하여 room 4개 생성
		printf("Room %d 생성 완료\n", i);
	}
}

void IOCP_CORE::CheckThisCPUcoreCount()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	cpuCore = static_cast<int>(si.dwNumberOfProcessors);
	printf("CPU Core Count = %d, threads = %d\n", cpuCore, cpuCore);
}

void IOCP_CORE::IOCP_MakeWorkerThreads()
{
	worker_threads.reserve(cpuCore);

	for (int i = 0; i < cpuCore; ++i)
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
	while (!ServerShutdown) {
		DWORD key;
		DWORD iosize;
		OVLP_EX* my_overlap;

		BOOL result = GetQueuedCompletionStatus(
			g_hIocp, &iosize, (PULONG_PTR)&key,
			reinterpret_cast<LPOVERLAPPED*>(&my_overlap),
			INFINITE
		);

		auto client_it = g_players.find(key);
		if (client_it == g_players.end()) {
			continue;
		}
		PLAYER_INFO* user = client_it->second;

		// 수신 오류 or 0바이트 -> 연결 끊김
		if (!result || iosize == 0) {
			if (!result) {
				int err_no = WSAGetLastError();
				IOCP_ErrorDisplay("WorkerThread::GQCS", err_no, __LINE__);
			}
			DisconnectClient(key);
			printf("[ No. %3u ] Disconnected\n", key);
			continue;
		}

		if (my_overlap->operation == OP_SERVER_RECV) {
			// 새로 수신된 데이터: user->recv_overlap.iocp_buffer ~ iosize
			char* buf_ptr = reinterpret_cast<char*>(user->recv_overlap.iocp_buffer);
			int remained = iosize;

			// 남은 바이트를 user->packet_buff에 누적
			while (remained > 0) {
				int space = MAX_PACKET_SIZE - user->previous_size;
				int copySize = min(remained, space);

				memcpy(user->packet_buff + user->previous_size, buf_ptr, copySize);
				user->previous_size += copySize;
				buf_ptr += copySize;
				remained -= copySize;

				// 4바이트 길이 헤더 기반 패킷 파싱
				while (true) {
					// 최소 4바이트 있어야 길이 확인 가능
					if (user->previous_size < 4) {
						break;
					}

					// 패킷 길이 읽기
					uint32_t protobufSize = 0;
					memcpy(&protobufSize, user->packet_buff, 4);

					// 아직 패킷이 다 안 들어왔으면 대기
					if (user->previous_size < (4 + protobufSize)) {
						break;
					}

					// 이제 (4 + protobufSize) 바이트가 하나의 패킷
					// -> 이 구간을 IOCP_ProcessPacket에 넘긴다
					bool parseOk = IOCP_ProcessPacket(
						key,
						user->packet_buff + 4,  // 헤더 뒤부터
						protobufSize           // 실제 Protobuf 데이터 길이
					);

					// leftover(남은 데이터) 이동
					int leftover = user->previous_size - (4 + protobufSize);
					if (leftover > 0) {
						memmove(user->packet_buff, user->packet_buff + (4 + protobufSize), leftover);
					}
					user->previous_size = leftover;

					if (!parseOk) {
						// 패킷 파싱 실패 시 처리(로그 등)
					}
				}
			}

			// 다음 수신
			DWORD flags = 0;
			WSARecv(user->s, &user->recv_overlap.wsabuf, 1, NULL, &flags,
				&user->recv_overlap.original_overlap, NULL);
		}
		else if (my_overlap->operation == OP_SERVER_SEND) {
			// 송신 완료
			delete my_overlap;
		}
		else {
			printf("Unknown overlap operation\n");
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
		SOCKET client_sock = WSAAccept(listen_sock, reinterpret_cast<sockaddr*>(&clientaddr), &addrlen, NULL, NULL);
		if (INVALID_SOCKET == client_sock) {
			int err_no = WSAGetLastError();
			IOCP_ErrorDisplay("Accept::WSAAccept", err_no, __LINE__);
			continue; // 무한루프 대신 continue
		}

		playerIndex += 1;
		printf("[ No. %3u ] Client IP = %s, Port = %d is Connected\n",
			playerIndex, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// Nagle 비활성화 (선택사항)
		BOOL bNoDelay = TRUE;
		retval = setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&bNoDelay, sizeof(bNoDelay));
		if (retval == SOCKET_ERROR) {
			int err_no = WSAGetLastError();
			IOCP_ErrorDisplay("setsockopt::TCP_NODELAY", err_no, __LINE__);
		}

		// ---- "길이(4) + playerIndex(4)" 형태로 8바이트 전송 ---
		{
			uint32_t packetSize = sizeof(playerIndex); // 4
			uint32_t totalSize = 4 + packetSize;       // 8

			char sendBuf[8] = { 0 };
			memcpy(sendBuf, &packetSize, 4);
			memcpy(sendBuf + 4, &playerIndex, 4);

			WSABUF wsabuf;
			wsabuf.buf = sendBuf;
			wsabuf.len = totalSize; // 8

			DWORD sent = 0;
			OVERLAPPED sendOverlap;
			ZeroMemory(&sendOverlap, sizeof(sendOverlap));

			retval = WSASend(client_sock, &wsabuf, 1, &sent, 0, &sendOverlap, NULL);
			if (retval == SOCKET_ERROR) {
				int err_no = WSAGetLastError();
				IOCP_ErrorQuit(L"id send()", err_no);
			}
		}

		// ----- IOCP 등록 -----
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), g_hIocp, playerIndex, 0);

		// PLAYER_INFO 객체 생성
		PLAYER_INFO* user = new PLAYER_INFO;
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

		// 첫 수신 준비
		DWORD flags = 0;
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
	std::lock_guard<std::mutex> lock(g_players_mutex);
	auto it = g_players.find(id);
	if (it != g_players.end()) {
		PLAYER_INFO* user = it->second;

		closesocket(user->s);
		delete user;
		g_players.erase(it);
	}
}


void IOCP_CORE::IOCP_SendPacket(unsigned int id, const char* serializedData, size_t dataSize)
{
	auto it = g_players.find(id);
	if (it == g_players.end() || !(it->second->connected)) {
		return;
	}

	PLAYER_INFO* user = it->second;

	// 1) [4바이트 길이 + 실제데이터] 준비
	uint32_t packetSize = (uint32_t)dataSize;
	uint32_t totalSize = 4 + packetSize;
	// iocp_buffer 크기가 totalSize보다 크다고 가정 (예: MAX_PACKET_SIZE 확인)

	OVLP_EX* over = new OVLP_EX;
	memset(over, 0, sizeof(OVLP_EX));
	over->operation = OP_SERVER_SEND;

	// a) 4바이트 길이 기록
	memcpy(over->iocp_buffer, &packetSize, sizeof(packetSize));
	// b) 그 뒤에 실제 데이터
	memcpy(over->iocp_buffer + 4, serializedData, dataSize);

	over->wsabuf.buf = reinterpret_cast<char*>(over->iocp_buffer);
	over->wsabuf.len = totalSize; // 4 + dataSize

	DWORD flags = 0;
	int retval = WSASend(user->s, &over->wsabuf, 1, NULL, flags, &over->original_overlap, NULL);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			IOCP_ErrorDisplay("SendPacket::WSASend", err_no, __LINE__);
			// 필요 시 delete over; (메모리 해제)
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
}
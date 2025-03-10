#pragma once

#include"LoginManager.h"
#include "Room.h"

class Room;

using OVLP_EX = struct Overlap_ex {
	OVERLAPPED original_overlap;
	int operation;
	WSABUF wsabuf;
	Packet iocp_buffer[MAX_BUF_SIZE];
};

using PLAYER_INFO = struct Client_INFO {
	SOCKET s;
	unsigned int id;
	bool connected;
	OVLP_EX recv_overlap;
	int packet_size;
	int previous_size;
	Packet packet_buff[MAX_BUF_SIZE];
	bool isInGame;
	bool ready;
	std::string username;
	int room_num;
	int player_num;
};

extern std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
extern std::unordered_map<unsigned int, bool> players_ready;
extern std::unordered_map<int, Room*> rooms;
extern std::mutex g_players_mutex;

class IOCP_CORE
{
public:
	IOCP_CORE();
	~IOCP_CORE();

	void IOCP_GetServerIpAddress();
	void IOCP_Initialize();
	void CheckThisCPUcoreCount();

	void IOCP_MakeWorkerThreads();
	void IOCP_WorkerThread();
	void IOCP_AcceptThread();

	void DisconnectClient(unsigned int clientId);

	bool IOCP_ProcessPacket(int id, Packet* buffer, int bufferSize);
	void IOCP_SendPacket(unsigned int id, const char* serializedData, size_t dataSize);

	void IOCP_ErrorDisplay(const char *msg, int err_no, int line);
	void IOCP_ErrorQuit(const wchar_t *msg, int err_no);

	bool CheckAllPlayersReady();

private:
	HANDLE g_hIocp;	
	int cpuCore;

	LoginManager loginmanager;

	vector<thread*> worker_threads;
	thread timer_thread;

	bool ServerShutdown{ false };

	unsigned int playerIndex{ UINT_MAX };
};
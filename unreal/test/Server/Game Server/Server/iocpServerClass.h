#pragma once
#include "Common.h"
#include "ZombieController.h"
#include "Zombie.h"


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


class ZombieController;


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
};

struct Player_Location {
	float x, y, z;
};

extern std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
extern std::unordered_map<int, Player_Location> playerLocations;

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

	void Timer_Thread();
	void Zombie_BT_Thread();
	
	//Player_Location playerlocation;	//지우기

	void Zombie_BT_Initialize();
	void ServerOn();

	//서버가 먼저 켜지고 좀비 BT가 실행되도록 하기위해 사용 
	bool bServerOn;

	//플레이어 인스턴스
	Player* player;				// 지우기
	//좀비 벡터
	vector<Zombie> zombie;

	//======Zombie_BT 선언======

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

	//[Task] 선언

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer;
	//[HasInvestigated-Task]
	THasInvestigated* t_hasinvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation;

	//<Selector CanSeePlayer> 가 가지는 Task들

	//[CanNotAttack-Task]
	TCanNotAttack* t_cannotattack;
	//[CanAttack-Task]
	TCanAttack* t_canattack;

	//{Sequence} 가 가지는 Task들

	//[MoveTo-Task]
	TMoveTo* t_moveto;
	//[Attack-Task]
	TAttack* t_attack;
	
	//==========================



private:
	HANDLE g_hIocp;	
	int cpuCore;

	vector<thread*> worker_threads;
	thread timer_thread;

	thread zombie_thread;

	bool ServerShutdown{ false };

	unsigned int playerIndex{ UINT_MAX };

	ZombieController* zombieclass;

	int GameTime = 0;

	bool b_Timer = false;
};
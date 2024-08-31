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
	
	//Player_Location playerlocation;	//�����

	void Zombie_BT_Initialize();
	void ServerOn();

	//������ ���� ������ ���� BT�� ����ǵ��� �ϱ����� ��� 
	bool bServerOn;

	//�÷��̾� �ν��Ͻ�
	Player* player;				// �����
	//���� ����
	vector<Zombie> zombie;

	//======Zombie_BT ����======

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

	//[Task] ����

	//<Selector Detact> �� ������ Task��

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer;
	//[HasInvestigated-Task]
	THasInvestigated* t_hasinvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation;

	//<Selector CanSeePlayer> �� ������ Task��

	//[CanNotAttack-Task]
	TCanNotAttack* t_cannotattack;
	//[CanAttack-Task]
	TCanAttack* t_canattack;

	//{Sequence} �� ������ Task��

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
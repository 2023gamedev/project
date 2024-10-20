#pragma once

#include "Common.h"

#include "ZombieController.h"
#include "ItemController.h"

#include "Player.h"


// BT
#include "Task.h"
#include "Selector.h"
#include "Sequence.h"

#include "CanSeePlayer.h"
#include "HasShouting.h"
#include "HasFootSound.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"
//#include "CanAttack.h"
//#include "CanNotAttack.h"
#include "Attack.h"
//#include "MoveTo.h"


class ZombieController;
class Zombie;
class ZombiePathFinder;
class ItemController;

class TMoveTo;	// 전방 선언 -> static 변수 BT_INTERVAL 을 MoveTo 클래스에서 사용하기 위해 (순환 포함 문제를 피하기 위해)
class TCanAttack;
class TCanNotAttack;

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
	int pingcnt = 0;
	
	// 전송 대기열 추가
	std::queue<std::string> sendQueue;  // 전송할 데이터를 담는 대기열
	std::mutex sendMutex;  // 큐 접근 제어용 락
	bool isSending = false;  // 현재 전송 중인지 여부를 나타내는 플래그
};

extern std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
extern std::unordered_map<int, Player> playerDB;

struct TupleHash {
	size_t operator()(const tuple<float, float, float>& t) const {
		size_t h1 = hash<float>{}(get<0>(t));
		size_t h2 = hash<float>{}(get<1>(t));
		size_t h3 = hash<float>{}(get<2>(t));
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct TupleEqual {
	bool operator()(const tuple<float, float, float>& t1, const tuple<float, float, float>& t2) const {
		return t1 == t2;
	}
};


extern std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapB2;
extern std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapB1;
extern std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapF1;
extern std::unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> g_EdgesMapF2;

extern std::vector<tuple<float, float, float>> g_valispositionsB2;
extern std::vector<tuple<float, float, float>> g_valispositionsB1;
extern std::vector<tuple<float, float, float>> g_valispositionsF1;
extern std::vector<tuple<float, float, float>> g_valispositionsF2;



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
	void IOCP_SendNextPacket(PLAYER_INFO* user);
	void IOCP_SendPacket(unsigned int id, const char* serializedData, size_t dataSize);

	void IOCP_ErrorDisplay(const char *msg, int err_no, int line);
	void IOCP_ErrorQuit(const wchar_t *msg, int err_no);

	void Timer_Thread();
	void Zombie_BT_Thread();

	bool LoadEdgesMap(const string& filePath,
		vector<tuple<float, float, float>>& positions,
		unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash>& EdgesMap);

	void SendPingToClients();

	void Zombie_BT_Initialize();
	void ServerOn();

	//서버가 먼저 켜지고 좀비 BT가 실행되도록 하기위해 사용 
	bool bServerOn;

	//좀비 벡터
	vector<Zombie*> zombieDB;

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
	//{Sequence-HasShouting}
	Sequence seq_hasshouting;
	//{Sequence-HasFootSound}
	Sequence seq_hasfootsound;
	//{Sequence-HasInvestigated}
	Sequence seq_hasinvestigated;
	//{Sequence-NotHasLastKnownPlayerLocation}
	Sequence seq_nothaslastknownplayerlocation;

	//[Task] 선언

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer;
	//[HasShouting-Task]
	THasShouting* t_hasshouting;
	//[HasFootSound-Task]
	THasFootSound* t_hasfootsound;
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
	
	static float BT_INTERVAL;		// BT 작동 인터벌 설정

	static std::chrono::duration<float> BT_deltaTime;

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
	ItemController* itemclass;

	float GameTime = 0.f;

	bool b_Timer = false;
};
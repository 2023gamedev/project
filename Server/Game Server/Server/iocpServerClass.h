#pragma once

#include "Common.h"

#include "ZombieController.h"
#include "ItemController.h"

#include "Player.h"

#include <concurrent_queue.h>
#include <atomic>


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


#define ENABLE_BT_LOG	// 전처리기 디렉티브 활성화 (좀비 BT 관련 로그 cout 찍기 플래그)
//#undef ENABLE_BT_LOG	// 비활성화

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
	char iocp_buffer[MAX_BUF_SIZE];
};

using PLAYER_INFO = struct Client_INFO {
	SOCKET s;
	unsigned int id;
	bool connected;
	OVLP_EX recv_overlap;
	int packet_size;
	int previous_size;
	char packet_buff[MAX_BUF_SIZE];
	bool isInGame;
	int pingcnt = 0;
	bool send_zombie = false;
	bool send_item = false;
	bool send_car = false;
	
	// 전송 대기열 추가
	Concurrency::concurrent_queue<std::string> sendQueue;  // 전송할 데이터를 담는 대기열
	Concurrency::concurrent_queue<std::string> recvQueue;  // 수신한 데이터를 담는 대기열
	std::mutex sendMutex;  // 큐 접근 제어용 락
	atomic<bool> isSending = false;  // 현재 전송 중인지 여부를 나타내는 플래그
};

struct Vector3D {
	float x, y;
	FLOOR floor;
};

extern std::unordered_map<unsigned int, PLAYER_INFO*> g_players;
extern std::unordered_map<int, Player> playerDB;

extern std::unordered_map<int, Player> playerDB_BT;

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

// ostream 연산자 오버로딩 선언
std::ostream& operator<<(std::ostream& os, FLOOR floor);


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

	bool IOCP_ProcessPacket(int id, const std::string& packet);
	void IOCP_SendNextPacket(PLAYER_INFO* user);
	void IOCP_SendPacket(unsigned int id, const char* serializedData, size_t dataSize);

	void IOCP_ErrorDisplay(const char *msg, int err_no, int line);
	void IOCP_ErrorQuit(const wchar_t *msg, int err_no);

	void Timer_Thread();
	void Zombie_BT_Thread();

	bool LoadEdgesMap(const string& filePath,
		vector<tuple<float, float, float>>& positions,
		unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash>& EdgesMap);

	bool UpdateEdgesMap(const string& originalFilePath, const string& copiedFilePath);

	void SendPingToClients();

	void Zombie_BT_Initialize();
	void ServerOn();

	//서버가 먼저 켜지고 좀비 BT가 실행되도록 하기위해 사용 
	bool bServerOn;

	//좀비 벡터
	vector<Zombie*> zombieDB;
	vector<Zombie*> zombieDB_BT;

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

	Vector3D Escape_Location;
	bool b_IsEscaping = false;
	int Escape_Root;
	string Root_Open_Player;

	int roofkey_cnt = 0;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GStruct.pb.h"
#include "LStruct.pb.h"
#include <concurrent_queue.h>
#include <mutex>

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"


constexpr int BUFSIZE = 8192;

enum class ServerType { LOBBY_SERVER, GAME_SERVER };


/**
 *
 */

struct PlayerData
{
	uint32 PlayerId;
	FVector Location;
	FRotator Rotation;
	uint32 charactertype;
	std::string username;
	float hp;

	// 기본 생성자 명시적 정의
	PlayerData() : PlayerId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), charactertype(0), username(""), hp(100) {}

	// 매개변수가 있는 생성자
	PlayerData(uint32 InPlayerId, FVector InLocation, FRotator InRotation, uint32 Incharactertype, std::string Inusername, float Inhp)
		: PlayerId(InPlayerId), Location(InLocation), Rotation(InRotation), charactertype(Incharactertype), username(Inusername), hp(Inhp) {}
};

struct ZombieData
{
	uint32 ZombieId;
	FVector Location;
	FRotator Rotation;
	uint32 ZombieType;

	// 기본 생성자 명시적 정의
	ZombieData() : ZombieId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), ZombieType(0) {}

	// 매개변수가 있는 생성자
	ZombieData(uint32 InZombieId, FVector InLocation, FRotator InRotation, uint32 InZombieType)
		: ZombieId(InZombieId), Location(InLocation), Rotation(InRotation), ZombieType(InZombieType) {}
};

struct PlayerAttack
{
	uint32 PlayerId;
	bool b_attack;

	PlayerAttack() : PlayerId(0), b_attack(false) {}

	PlayerAttack(uint32 InPlayerId, bool Inb_attack)
		: PlayerId(InPlayerId), b_attack(Inb_attack) {}
};

struct EquipItem
{
	uint32 PlayerId;
	std::string Itemname;
	uint32 Itemtype;

	EquipItem() : PlayerId(0), Itemname(""), Itemtype(0) {}

	EquipItem(uint32 InPlayerId, std::string InItemname, uint32 InItemtype)
		: PlayerId(InPlayerId), Itemname(InItemname), Itemtype(InItemtype) {}
};

struct DetachItem
{
	uint32 PlayerId;
	uint32 Itemtype;

	DetachItem() : PlayerId(0), Itemtype(0) {}

	DetachItem(uint32 InPlayerId, uint32 InItemtype)
		: PlayerId(InPlayerId), Itemtype(InItemtype) {}
};

struct PlayerRun
{
	uint32 PlayerId;
	uint32 b_run;

	PlayerRun() : PlayerId(0), b_run(0) {}

	PlayerRun(uint32 InPlayerId, uint32 Inb_run)
		: PlayerId(InPlayerId), b_run(Inb_run) {}
};

struct PlayerJump
{
	uint32 PlayerId;

	PlayerJump() : PlayerId(0) {}

	PlayerJump(uint32 InPlayerId)
		: PlayerId(InPlayerId) {}
};

struct CharacterSelect
{
	uint32 PlayerId;
	uint32 Character_type;

	CharacterSelect() : PlayerId(0), Character_type(0) {}

	CharacterSelect(uint32 InPlayerId, uint32 InCharacter_type)
		: PlayerId(InPlayerId), Character_type(InCharacter_type) {}
};

struct ZombiePath
{
	uint32 ZombieId;
	std::vector<std::tuple<float, float, float>> Path1;
	std::vector<std::tuple<float, float, float>> Path2;
	FVector Location;

	ZombiePath() : ZombieId(0), Path1(), Path2(), Location(FVector::ZeroVector) {}

	ZombiePath(uint32 InZombieId, const std::vector<std::tuple<float, float, float>>& InPath1, const std::vector<std::tuple<float, float, float>>& InPath2, const FVector& InLocation)
		: ZombieId(InZombieId), Path1(InPath1), Path2(InPath2), Location(InLocation) {}
};

struct ZombieHP
{
	uint32 ZombieId;
	float Damage;

	ZombieHP() : ZombieId(0), Damage(0) {}

	ZombieHP(uint32 InZombieId, float InDamage)
		: ZombieId(InZombieId), Damage(InDamage) {}
};

struct ZombieAttack
{
	uint32 ZombieId;
	uint32 PlayerId;

	ZombieAttack() : ZombieId(0), PlayerId(0) {}

	ZombieAttack(uint32 InZombieId, uint32 InPlayerId)
		: ZombieId(InZombieId), PlayerId(InPlayerId) {}
};

struct Set_Item
{
	uint32 itemid;
	std::string itemname;
	uint32 itemclass;
	std::string texture_path;
	uint32 count;
	uint32 floor;
	FVector itempos;

	Set_Item() : itemid(0), itemname(""), itemclass(0), texture_path(""), count(0), floor(0), itempos(FVector::ZeroVector) {}

	Set_Item(uint32 Initemid, std::string Initemname, uint32 Initemclass, std::string Intexture_path, uint32 Incount, uint32 Infloor, FVector Initempos)
		: itemid(Initemid), itemname(Initemname), itemclass(Initemclass), texture_path(Intexture_path), count(Incount), floor(Infloor), itempos(Initempos) {}
};

struct Set_Car
{
	uint32 carid;
	std::string carname;
	FVector carpos;
	FRotator carrotator;
	std::string carkeyname;

	Set_Car()
		: carid(0), carname(""), carpos(FVector::ZeroVector),
		carrotator(FRotator::ZeroRotator), carkeyname("") {}

	Set_Car(uint32 id, const std::string& name, const FVector& pos, const FRotator& rot, const std::string& keyname)
		: carid(id), carname(name), carpos(pos), carrotator(rot), carkeyname(keyname) {}
};

struct Drop_Item
{
	uint32 itemid;
	std::string itemname;
	uint32 itemclass;
	std::string texture_path;
	uint32 count;
	FVector itempos;

	Drop_Item() : itemid(0), itemname(""), itemclass(0), texture_path(""), count(0), itempos(FVector::ZeroVector) {}

	Drop_Item(uint32 Initemid, std::string Initemname, uint32 Initemclass, std::string Intexture_path, uint32 Incount, FVector Initempos)
		: itemid(Initemid), itemname(Initemname), itemclass(Initemclass), texture_path(Intexture_path), count(Incount), itempos(Initempos) {}
};

struct Destroy_Item
{
	uint32 itemid;
	uint32 playerid;

	Destroy_Item() : itemid(0), playerid(0) {}

	Destroy_Item(uint32 Initemid, uint32 Inplayerid)
		: itemid(Initemid), playerid(Inplayerid) {}

};

struct Get_Key
{
	uint32 itemid;
	uint32 playerid;

	Get_Key() : itemid(0), playerid(0) {}

	Get_Key(uint32 Initemid, uint32 Inplayerid)
		: itemid(Initemid), playerid(Inplayerid) {}
};

struct Escape_Root
{
	uint32 playerid;
	uint32 root;

	Escape_Root() : playerid(0), root(0) {}

	Escape_Root(uint32 Inplayerid, uint32 Inroot)
		: playerid(Inplayerid), root(Inroot) {}
};

struct Chatting
{
	uint32 playerid;
	FString chat;

	Chatting() : playerid(0), chat("") {}

	Chatting(uint32 Inplayerid, FString Inchat)
		: playerid(Inplayerid), chat(Inchat) {}
};

struct JoinPlayer
{
	std::string username;
	uint32 playerid;
	uint32 roomid;


	JoinPlayer() : username(""), playerid(0), roomid(0) {}

	JoinPlayer(std::string Inusername, uint32 Inplayerid, uint32 Inroomid)
		:username(Inusername), playerid(Inplayerid), roomid(Inroomid) {}
};

struct LeavePlayer
{
	std::string username;
	uint32 playerid;


	LeavePlayer() : username(""), playerid(0) {}

	LeavePlayer(std::string Inusername, uint32 Inplayerid)
		:username(Inusername), playerid(Inplayerid) {}
};

struct WaitingReady
{
	uint32 playerid;
	bool ready;

	WaitingReady() : playerid(0), ready(false) {}

	WaitingReady(uint32 Inplayerid, bool Inready)
		: playerid(Inplayerid), ready(Inready) {}
};

struct SelectReady
{
	uint32 playerid;
	uint32 player_num;
	bool ready;

	SelectReady() : playerid(0), ready(false) {}

	SelectReady(uint32 Inplayerid, uint32 Inplayer_num, bool Inready)
		: playerid(Inplayerid), player_num(Inplayer_num), ready(Inready) {}
};

struct GameClear
{
	uint32 root;
	uint32 alive_players;
	uint32 dead_players;
	std::string open_player;
	uint32 my_kill_count;
	uint32 best_kill_count;
	std::string best_kill_player;

	GameClear()
		: root(0), alive_players(0), dead_players(0), open_player(""),
		my_kill_count(0), best_kill_count(0), best_kill_player("") {}

	GameClear(uint32 Inroot, uint32 Inalive_players, uint32 Indead_players,
		const std::string& Inopen_player, uint32 Inmy_kill_count,
		uint32 Inbest_kill_count, const std::string& Inbest_kill_player)
		: root(Inroot), alive_players(Inalive_players), dead_players(Indead_players),
		open_player(Inopen_player), my_kill_count(Inmy_kill_count),
		best_kill_count(Inbest_kill_count), best_kill_player(Inbest_kill_player) {}
};

struct SliceVector
{
	uint32 zombieid;
	FVector location;
	FVector position;
	FVector normal;
	FVector impulse;

	SliceVector()
		: zombieid(0), location(FVector()), position(FVector()),
		normal(FVector()), impulse(FVector()) {}

	SliceVector(uint32 InZombieId, const FVector& InLocation, const FVector& InPosition,
		const FVector& InNormal, const FVector& InImpulse)
		: zombieid(InZombieId), location(InLocation), position(InPosition),
		normal(InNormal), impulse(InImpulse) {}
};

class UProGameInstance;

class PROJECT_API ClientSocket : public FRunnable
{
public:
	ClientSocket(UProGameInstance* Inst);
	~ClientSocket() override;

	SOCKET Socket;
	//UPROPERTY(VisibleAnywhere)
	uint32 MyPlayerId = 0;		// 실제 서버에서 관리하는 플레이어 DB에서 나의 인덱스 번호
	uint32 OtherPlayerId = 0;
	uint32 ZombieId = 0;
	uint32 MyRoomId = 0;
	std::string MyUserName;

	Concurrency::concurrent_queue<PlayerData> Q_player;
	Concurrency::concurrent_queue<PlayerAttack> Q_pattack;
	Concurrency::concurrent_queue<ZombieData> Q_zombie;
	Concurrency::concurrent_queue<EquipItem> Q_eitem;
	Concurrency::concurrent_queue<DetachItem> Q_detachitem;
	Concurrency::concurrent_queue<PlayerRun> Q_run;
	Concurrency::concurrent_queue<PlayerJump> Q_jump;
	Concurrency::concurrent_queue<CharacterSelect> Q_select;
	Concurrency::concurrent_queue<ZombiePath> Q_path;
	Concurrency::concurrent_queue<bool> Q_ping;
	Concurrency::concurrent_queue<ZombieHP> Q_zhp;
	Concurrency::concurrent_queue<ZombieAttack> Q_zattack;
	Concurrency::concurrent_queue<Set_Item> Q_setitem;
	Concurrency::concurrent_queue<Set_Car> Q_setcar;
	Concurrency::concurrent_queue<Destroy_Item> Q_ditem;
	Concurrency::concurrent_queue<Get_Key> Q_getkey;
	Concurrency::concurrent_queue<Escape_Root> Q_escape;
	Concurrency::concurrent_queue<Chatting> Q_chat;
	Concurrency::concurrent_queue<JoinPlayer> Q_jplayer;
	Concurrency::concurrent_queue<LeavePlayer> Q_lplayer;
	Concurrency::concurrent_queue<WaitingReady> Q_wready;
	Concurrency::concurrent_queue<SelectReady> Q_sready;
	Concurrency::concurrent_queue<bool> Q_wAllready;
	Concurrency::concurrent_queue<GameClear> Q_gclear;
	Concurrency::concurrent_queue<Drop_Item> Q_dropitem;
	Concurrency::concurrent_queue<SliceVector> Q_slicevector;


	virtual bool Init() override;
	void StartRecv();
	virtual uint32 Run() override;
	void ProcessRecvQueue();
	void ProcessPacket(const std::vector<char>& buffer);
	virtual void Exit() override;
	bool ConnectServer(ServerType serverType);

	bool Send(const int SendSize, void* SendData);
	void StartSend();

	uint32 GetMyPlayerId() const;

	float Timer = 0.f;

	bool b_run = false;

	ServerType CurrentServerType;

	bool recv_login = false;
	bool recv_register = false;
	bool b_login = false;
	bool b_register = false;

	bool b_allready = false;

	bool recv_zombiepacket = false;
	bool recv_itempacket = false;
	bool recv_carpacket = false;

	std::vector<char> recvBufferCache;


private:

	HANDLE hIocp;  // I/O 완료 포트 핸들
	//std::mutex recvMutex;  // 수신 큐를 보호하기 위한 뮤텍스
	//std::mutex playerQueueMutex;

	std::vector<std::thread> workerThreads;
	std::thread sendThread;

	Concurrency::concurrent_queue<std::vector<char>> recvQueue;
	Concurrency::concurrent_queue<std::vector<char>> sendQueue;

	WSAOVERLAPPED recvOverlap = {};  // 수신 작업에 사용할 OVERLAPPED 구조체
	WSABUF recvBuffer;               // 수신 버퍼 설정
	char recvData[BUFSIZE];          // 수신 데이터를 저장할 버퍼


	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;

	std::atomic<bool> isSending = false;
};
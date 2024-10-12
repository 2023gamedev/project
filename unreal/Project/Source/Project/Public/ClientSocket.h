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


constexpr int BUFSIZE = 1024;

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
	float hp;

	// 기본 생성자 명시적 정의
	PlayerData() : PlayerId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), charactertype(0), hp(100) {}

	// 매개변수가 있는 생성자
	PlayerData(uint32 InPlayerId, FVector InLocation, FRotator InRotation, uint32 Incharactertype, float Inhp)
		: PlayerId(InPlayerId), Location(InLocation), Rotation(InRotation), charactertype(Incharactertype), hp(Inhp) {}
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
	std::vector<std::tuple<float, float, float>> Path;
	FVector Location;

	ZombiePath() : ZombieId(0), Path(), Location(FVector::ZeroVector) {}

	ZombiePath(uint32 InZombieId, const std::vector<std::tuple<float, float, float>>& InPath, const FVector& InLocation)
		: ZombieId(InZombieId), Path(InPath), Location(InLocation) {}
};

struct ZombieHP
{
	uint32 ZombieId;
	uint32 Hp;

	ZombieHP() : ZombieId(0), Hp(0) {}

	ZombieHP(uint32 InZombieId, uint32 InHp)
		: ZombieId(InZombieId), Hp(InHp) {}
};

struct ZombieAttack
{
	uint32 ZombieId;
	uint32 PlayerId;

	ZombieAttack() : ZombieId(0), PlayerId(0) {}

	ZombieAttack(uint32 InZombieId, uint32 InPlayerId)
		: ZombieId(InZombieId), PlayerId(InPlayerId) {}
};


class UProGameInstance;

class PROJECT_API ClientSocket : public FRunnable
{
public:
	ClientSocket(UProGameInstance* Inst);
	~ClientSocket() override;

	SOCKET Socket;
	uint32 MyPlayerId = 0;
	uint32 OtherPlayerId = 0;
	uint32 ZombieId = 0;

	Concurrency::concurrent_queue<PlayerData> Q_player;
	Concurrency::concurrent_queue<PlayerAttack> Q_pattack;
	Concurrency::concurrent_queue<ZombieData> Q_zombie;
	Concurrency::concurrent_queue<EquipItem> Q_eitem;
	Concurrency::concurrent_queue<PlayerRun> Q_run;
	Concurrency::concurrent_queue<PlayerJump> Q_jump;
	Concurrency::concurrent_queue<CharacterSelect> Q_select;
	Concurrency::concurrent_queue<ZombiePath> Q_path;
	Concurrency::concurrent_queue<bool> Q_ping;
	Concurrency::concurrent_queue<ZombieHP> Q_zhp;
	Concurrency::concurrent_queue<ZombieAttack> Q_zattack;


	virtual bool Init() override;
	void StartRecv();
	virtual uint32 Run() override;
	void ProcessRecvQueue();
	void ProcessPacket(const std::vector<char>& buffer);
	virtual void Exit() override;
	bool ConnectServer(ServerType serverType);
	bool Send(const int SendSize, void* SendData);

	uint32 GetMyPlayerId() const;

	float Timer = 0.f;

	bool b_run = false;

	ServerType CurrentServerType;

	bool recv_login = false;
	bool recv_register = false;
	bool b_login = false;
	bool b_register = false;

	bool b_allready = false;



private:

	HANDLE hIocp;  // I/O 완료 포트 핸들
	std::mutex recvMutex;  // 수신 큐를 보호하기 위한 뮤텍스
	std::mutex playerQueueMutex;
	std::queue<std::vector<char>> recvQueue;  // 수신된 데이터를 담을 큐

	WSAOVERLAPPED recvOverlap = {};  // 수신 작업에 사용할 OVERLAPPED 구조체
	WSABUF recvBuffer;               // 수신 버퍼 설정
	char recvData[BUFSIZE];          // 수신 데이터를 저장할 버퍼

	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;
};
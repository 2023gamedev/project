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
	uint32 zombietype;

	// 기본 생성자 명시적 정의
	ZombieData() : ZombieId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), zombietype(0) {}

	// 매개변수가 있는 생성자
	ZombieData(uint32 InZombieId, FVector InLocation, FRotator InRotation, uint32 Inzombietype)
		: ZombieId(InZombieId), Location(InLocation), Rotation(InRotation), zombietype(Inzombietype) {}
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
	bool b_run;

	PlayerRun() : PlayerId(0), b_run(false) {}

	PlayerRun(uint32 InPlayerId, bool Inb_run)
		: PlayerId(InPlayerId), b_run(Inb_run) {}
};

struct PlayerJump
{
	uint32 PlayerId;

	PlayerJump() : PlayerId(0) {}

	PlayerJump(uint32 InPlayerId)
		: PlayerId(InPlayerId) {}
};


class UProGameInstance;

class PROJECT_API ClientSocket : public FRunnable
{
public:
	ClientSocket(UProGameInstance* Inst);
	~ClientSocket() override;

	SOCKET Socket;
	uint32 MyPlayerId = 0;
	uint32 PlayerId = 0;
	uint32 ZombieId = 0;

	Concurrency::concurrent_queue<PlayerData> Q_player;
	Concurrency::concurrent_queue<PlayerAttack> Q_pattack;
	Concurrency::concurrent_queue<ZombieData> Q_zombie;
	Concurrency::concurrent_queue<EquipItem> Q_eitem;
	Concurrency::concurrent_queue<PlayerRun> Q_run;
	Concurrency::concurrent_queue<PlayerJump> Q_jump;
	

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	bool ConnectServer(ServerType serverType);
	bool Send(const int SendSize, void* SendData);

	uint32 GetMyPlayerId() const;

	uint32 Timer = 0;

	bool b_run = false;

	ServerType CurrentServerType;



private:

	std::mutex bufferMutex;
	std::mutex socketMutex;

	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;
};

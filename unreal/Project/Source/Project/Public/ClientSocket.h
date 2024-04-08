// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GStruct.pb.h"
#include "ProGamemode/ProGameInstance.h"
#include <concurrent_queue.h>
#include <mutex>

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"


constexpr int BUFSIZE = 1024;

/**
 *
 */

struct PlayerData
{
	uint32 PlayerId;
	FVector Location;
	FRotator Rotation;

	// 기본 생성자 명시적 정의
	PlayerData() : PlayerId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}

	// 매개변수가 있는 생성자
	PlayerData(uint32 InPlayerId, FVector InLocation, FRotator InRotation)
		: PlayerId(InPlayerId), Location(InLocation), Rotation(InRotation) {}
};

struct ZombieData
{
	uint32 ZombieId;
	FVector Location;
	FRotator Rotation;

	// 기본 생성자 명시적 정의
	ZombieData() : ZombieId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}

	// 매개변수가 있는 생성자
	ZombieData(uint32 InZombieId, FVector InLocation, FRotator InRotation)
		: ZombieId(InZombieId), Location(InLocation), Rotation(InRotation) {}
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
	Concurrency::concurrent_queue<ZombieData> Q_zombie;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	bool Send(const int SendSize, void* SendData);

	uint32 GetMyPlayerId() const;



private:

	std::mutex bufferMutex;
	std::mutex socketMutex;

	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;
};

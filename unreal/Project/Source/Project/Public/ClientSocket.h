// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GStruct.pb.h"
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
	uint32 charactertype;

	// �⺻ ������ ����� ����
	PlayerData() : PlayerId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), charactertype(0) {}

	// �Ű������� �ִ� ������
	PlayerData(uint32 InPlayerId, FVector InLocation, FRotator InRotation, uint32 Incharactertype)
		: PlayerId(InPlayerId), Location(InLocation), Rotation(InRotation), charactertype(Incharactertype) {}
};

struct ZombieData
{
	uint32 ZombieId;
	FVector Location;
	FRotator Rotation;
	uint32 zombietype;

	// �⺻ ������ ����� ����
	ZombieData() : ZombieId(0), Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), zombietype(0) {}

	// �Ű������� �ִ� ������
	ZombieData(uint32 InZombieId, FVector InLocation, FRotator InRotation, uint32 Inzombietype)
		: ZombieId(InZombieId), Location(InLocation), Rotation(InRotation), zombietype(Inzombietype) {}
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
	bool ConnectServer();
	bool Send(const int SendSize, void* SendData);

	uint32 GetMyPlayerId() const;



private:

	std::mutex bufferMutex;
	std::mutex socketMutex;

	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;
};

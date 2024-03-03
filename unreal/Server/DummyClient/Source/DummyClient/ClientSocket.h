// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GStruct.pb.h"
#include "DummyClientGameMode.h"
#include <concurrent_queue.h>


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

class DUMMYCLIENT_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	~ClientSocket() override;

	SOCKET Socket;
	uint32 MyPlayerId = 0;
	uint32 PlayerId = 0;

	Concurrency::concurrent_queue<PlayerData> Qbuffer;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	bool Send(const int SendSize, void* SendData);

	uint32 GetMyPlayerId() const;



private:

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;
};

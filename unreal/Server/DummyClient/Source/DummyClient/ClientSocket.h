// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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

class DUMMYCLIENT_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	~ClientSocket() override;

	SOCKET Socket;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	bool Send(const int SendSize, void* SendData);


private:

	FRunnableThread* Thread;

};

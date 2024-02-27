// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill();
		delete Thread;
	}
}

bool ClientSocket::Init()
{
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0)
	{
		return false;
	}

	Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET)
	{
		return false;
	}

	SOCKADDR_IN ServerAddr;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(7777);

	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	retval = connect(Socket, (sockaddr*)&ServerAddr, sizeof(sockaddr));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	if (retval == SOCKET_ERROR)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail")));
		return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Success!")));

		UE_LOG(LogNet, Display, TEXT("Socket Initialized"));
		return true;
	}
}

uint32 ClientSocket::Run()
{
	char RecvBuff[BUFSIZE];
	while (1)
	{
		int RecvLen = recv(Socket, reinterpret_cast<char*>(RecvBuff), BUFSIZE, 0);
		if (RecvLen > 0) // 데이터를 성공적으로 수신했을 경우
		{
			FString ReceivedData = FString(ANSI_TO_TCHAR(RecvBuff)).Left(RecvLen);

			UE_LOG(LogNet, Display, TEXT("Received Data: %s"), *ReceivedData);
		}
		else // SOCKET_ERROR 발생했을 경우
		{
			UE_LOG(LogNet, Warning, TEXT("Recv Error"));
			return 0;
		}
	}

	return 0;
}

void ClientSocket::Exit()
{
	if (Socket)
	{
		closesocket(Socket);
		WSACleanup();
	}
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	char buff[BUFSIZE];
	memcpy(buff, SendData, SendSize);

	int nSendLen = send(Socket, buff, SendSize, 0);
	UE_LOG(LogNet, Display, TEXT("Send Packet SIZE %d"), nSendLen);

	return true;
}

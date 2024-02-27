// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include <vector>

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
	std::vector<char> buffer; // 수신 데이터를 저장할 버퍼

	while (1)
	{
		char tempBuff[BUFSIZE]; // 임시 수신 버퍼
		int recvLen = recv(Socket, tempBuff, BUFSIZE, 0); // 데이터 수신

		if (!ReceivedPlayerId && recvLen > 0)
		{
			memcpy(&MyPlayerId, tempBuff, sizeof(MyPlayerId));

			UE_LOG(LogNet, Display, TEXT("Received MyPlayerId: %d"), MyPlayerId);

			ReceivedPlayerId = true; // PlayerId 수신 완료

			buffer.clear();

			continue;
		}

		else if (recvLen > 0)
		{
			// 수신된 데이터를 메인 버퍼에 추가
			buffer.insert(buffer.end(), tempBuff, tempBuff + recvLen);

			// Protobuf 메시지를 파싱 시도
			Protocol::TestPacket testPacket;
			if (testPacket.ParseFromArray(buffer.data(), buffer.size()))
			{
				UE_LOG(LogNet, Display, TEXT("Received: PlayerId=%d, Location=(X=%f, Y=%f, Z=%f)"),
					testPacket.playerid(),
					testPacket.x(),
					testPacket.y(),
					testPacket.z());

				PlayerId = testPacket.playerid();
				FVector NewLocation(testPacket.x(), testPacket.y(), testPacket.z());

				if (GameModeRef && PlayerId != MyPlayerId)
				{
					GameModeRef->UpdateOtherPlayer(PlayerId, NewLocation);
				}

				buffer.clear();
			}
			
		}
		else if (recvLen == 0) // 연결 종료
		{
			UE_LOG(LogNet, Warning, TEXT("Connection closed"));
			return 0;
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

	return true;
}

uint32 ClientSocket::GetMyPlayerId() const
{
	return MyPlayerId;
}



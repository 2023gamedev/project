// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GStruct.pb.h"
#include "LStruct.pb.h"
#include <concurrent_queue.h>
#include <mutex>
#include "HAL/Runnable.h"
#include "Sockets.h"
#include "Templates/SharedPointer.h"
#include "Containers/Queue.h"

class RecvWorker;
class SendWorker;
class PacketSession;

constexpr int BUFSIZE = 1024;

enum class ServerType { LOBBY_SERVER, GAME_SERVER };

/**
 *
 */

struct FPacketHeader
{
	uint32 PacketID;    // 패킷의 고유 ID
	uint32 PacketSize;  // 패킷의 전체 크기 (헤더 포함)

	// 기본 생성자
	FPacketHeader()
		: PacketID(0), PacketSize(0)
	{
	}

	// 생성자
	FPacketHeader(uint32 InPacketID, uint32 InPacketSize)
		: PacketID(InPacketID), PacketSize(InPacketSize)
	{
	}

	// 시리얼라이즈 함수 (Unreal에서 바이너리 스트림으로 읽고 쓰는 것을 지원)
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketID;
		Ar << Header.PacketSize;
		return Ar;
	}
};

struct SendBuffer
{
	SendBuffer(const void* Data, int32 InSize)
		: BufferData(), DataSize(InSize)
	{
		BufferData.Append(static_cast<const uint8*>(Data), InSize);
	}

	const uint8* Buffer() const { return BufferData.GetData(); }
	int32 GetSize() const { return DataSize; }

public:
	TArray<uint8> BufferData;
	int32 DataSize;
};

typedef TSharedPtr<SendBuffer> SendBufferRef;


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

struct CharacterSelect
{
	uint32 PlayerId;
	uint32 Character_type;

	CharacterSelect() : PlayerId(0), Character_type(0) {}

	CharacterSelect(uint32 InPlayerId, uint32 InCharacter_type)
		: PlayerId(InPlayerId), Character_type(InCharacter_type) {}
};


class UProGameInstance;

class PROJECT_API ClientSocket : public FRunnable
{
public:
	ClientSocket(UProGameInstance* Inst);
	~ClientSocket() override;

	FSocket* Socket;
	uint32 MyPlayerId = 0;
	uint32 PlayerId = 0;
	uint32 ZombieId = 0;

	Concurrency::concurrent_queue<PlayerData> Q_player;
	Concurrency::concurrent_queue<PlayerAttack> Q_pattack;
	Concurrency::concurrent_queue<ZombieData> Q_zombie;
	Concurrency::concurrent_queue<EquipItem> Q_eitem;
	Concurrency::concurrent_queue<PlayerRun> Q_run;
	Concurrency::concurrent_queue<PlayerJump> Q_jump;
	Concurrency::concurrent_queue<CharacterSelect> Q_select;
	

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool ConnectServer(ServerType serverType);
	bool Send(const int SendSize, void* SendData);

	void ProcessPackets();
	void HandleLobbyServerPacket(const Protocol::SC_Ready& Packet);
	void HandleGameServerPacket(const TArray<uint8>& Packet);

	uint32 GetMyPlayerId() const;

	uint32 Timer = 0;

	bool b_run = false;

	ServerType CurrentServerType;

	bool recv_login = false;
	bool recv_register = false;
	bool b_login = false;
	bool b_register = false;

	bool b_allready = false;



private:

	std::mutex bufferMutex;
	std::mutex socketMutex;

	UProGameInstance* gameInst;

	FRunnableThread* Thread;
	bool ReceivedPlayerId = false;

	TSharedPtr<RecvWorker> RecvThread;
	TSharedPtr<SendWorker> SendThread;
	TSharedPtr<PacketSession> Session;
};


class PROJECT_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	virtual ~RecvWorker() override;


	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	void Destroy();

private:

	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

	FSocket* Socket;
	TSharedPtr<class PacketSession> SessionRef;
	FRunnableThread* Thread;
	bool Running = true;
};


class SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	virtual ~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	void Destroy();

private:
	bool SendPacket(SendBufferRef SendBuffer);
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

	FSocket* Socket;
	TSharedPtr<class PacketSession> SessionRef;
	FRunnableThread* Thread;
	bool Running = true;
};


class PacketSession : public TSharedFromThis<PacketSession>
{
public:
	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};
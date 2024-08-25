// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "ProGamemode/ProGameInstance.h"

ClientSocket::ClientSocket(UProGameInstance* Inst)
	: gameInst(Inst)
{
	CurrentServerType = ServerType::GAME_SERVER;

	if (ConnectServer(ServerType::GAME_SERVER))
	{
		Session = MakeShareable(new PacketSession());
		RecvThread = MakeShareable(new RecvWorker(Socket, Session));
		SendThread = MakeShareable(new SendWorker(Socket, Session));
	}
}

ClientSocket::~ClientSocket()
{
	if (RecvThread.IsValid())
	{
		RecvThread->Destroy();
	}

	if (SendThread.IsValid())
	{
		SendThread->Destroy();
	}

	Exit();
}

bool ClientSocket::Init()
{
	UE_LOG(LogNet, Warning, TEXT("ClientSocket initialized"));
	return true;
}

uint32 ClientSocket::Run()
{
	return 0;
}

void ClientSocket::Exit()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

bool ClientSocket::ConnectServer(ServerType serverType)
{
	ReceivedPlayerId = false;

	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	if (!Socket)
	{
		return false;
	}

	int32 Port = (serverType == ServerType::LOBBY_SERVER) ? 7777 : 8888;

	TSharedRef<FInternetAddr> ServerAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	ServerAddr->SetIp(TEXT("127.0.0.1"), bIsValid);
	ServerAddr->SetPort(Port);

	bool bConnected = Socket->Connect(*ServerAddr);

	if (!bConnected)
	{
		UE_LOG(LogNet, Error, TEXT("Failed to connect to server"));
		return false;
	}
	else
	{
		UE_LOG(LogNet, Display, TEXT("Connected to server"));
		return true;
	}
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	if (Session.IsValid())
	{
		SendBufferRef Buffer = MakeShareable(new SendBuffer(SendData, SendSize));
		Session->SendPacketQueue.Enqueue(Buffer);
		return true;
	}
	return false;
}

void ClientSocket::ProcessPackets()
{
	TArray<uint8> Packet;
	while (Session->RecvPacketQueue.Dequeue(Packet))
	{
		Protocol::SC_Ready PacketReady;
		if (PacketReady.ParseFromArray(Packet.GetData(), Packet.Num()))
		{
			HandleLobbyServerPacket(PacketReady);
		}
		else if (CurrentServerType == ServerType::GAME_SERVER)
		{
			HandleGameServerPacket(Packet);
		}
	}
}

void ClientSocket::HandleLobbyServerPacket(const Protocol::SC_Ready& Packet)
{
	switch (Packet.type())
	{
	case 3: // Handle login packet
	{
		Protocol::SC_Login Login_Packet;
		if (Login_Packet.ParseFromArray(Packet.SerializeAsString().c_str(), Packet.ByteSizeLong())) {
			b_login = Login_Packet.b_login();
			recv_login = true;
		}
		break;
	}
	case 4: // Handle register packet
	{
		Protocol::SC_Register Register_Packet;
		if (Register_Packet.ParseFromArray(Packet.SerializeAsString().c_str(), Packet.ByteSizeLong())) {
			b_register = Register_Packet.b_register();
			recv_register = true;
		}
		break;
	}
	case 6: // Handle ready packet
	{
		if (Packet.allready())
		{
			b_allready = true;
			// 로비 서버 연결 종료
			Exit();

			// 게임 서버에 연결
			if (ConnectServer(ServerType::GAME_SERVER))
			{
				CurrentServerType = ServerType::GAME_SERVER;
				UE_LOG(LogNet, Display, TEXT("Connected to Game Server"));
			}
			else
			{
				UE_LOG(LogNet, Warning, TEXT("Failed to connect to Game Server"));
			}
		}
		break;
	}
	default:
		UE_LOG(LogNet, Warning, TEXT("Unknown Packet Type: %d"), Packet.type());
		break;
	}
}


void ClientSocket::HandleGameServerPacket(const TArray<uint8>& Packet)
{
	Protocol::Character tempCharacterPacket;
	if (tempCharacterPacket.ParseFromArray(Packet.GetData(), Packet.Num()))
	{
		// 메시지 타입 확인
		switch (tempCharacterPacket.packet_type())
		{
		case 1: // Character 메시지 타입 값
		{
			PlayerId = tempCharacterPacket.playerid();
			FVector NewLocation(tempCharacterPacket.x(), tempCharacterPacket.y(), tempCharacterPacket.z());
			FRotator NewRotation(tempCharacterPacket.pitch(), tempCharacterPacket.yaw(), tempCharacterPacket.roll());

			if (PlayerId != MyPlayerId) {
				Q_player.push(PlayerData(PlayerId, NewLocation, NewRotation, tempCharacterPacket.charactertype(), tempCharacterPacket.hp()));
			}
			break;
		}
		case 2: // Zombie 메시지 타입 값
		{
			Protocol::Zombie ZombiePacket;
			if (ZombiePacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				FVector NewLocation(ZombiePacket.x(), ZombiePacket.y(), ZombiePacket.z());
				FRotator NewRotation(ZombiePacket.pitch(), ZombiePacket.yaw(), ZombiePacket.roll());
				Q_zombie.push(ZombieData(ZombiePacket.zombieid(), NewLocation, NewRotation, ZombiePacket.zombietype()));
				UE_LOG(LogNet, Display, TEXT("push Zombie: ZombieId=%d"), ZombiePacket.zombieid());
			}
			break;
		}
		case 3: // Time 메시지 타입 값
		{
			Protocol::Time TimePacket;
			if (TimePacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				Timer = TimePacket.timer();
				UE_LOG(LogNet, Display, TEXT("Timer: %d"), Timer);
			}
			break;
		}
		case 4: // Character Attack 메시지 타입 값
		{
			Protocol::Character_Attack AttackPacket;
			if (AttackPacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				Q_pattack.push(PlayerAttack(AttackPacket.playerid(), AttackPacket.attack()));
			}
			break;
		}
		case 5: // Equip Item 메시지 타입 값
		{
			Protocol::Equip_Item EquipPacket;
			if (EquipPacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				Q_eitem.push(EquipItem(EquipPacket.playerid(), EquipPacket.itemname(), EquipPacket.itemtype()));
			}
			break;
		}
		case 6: // Run 메시지 타입 값
		{
			Protocol::run runPacket;
			if (runPacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				Q_run.push(PlayerRun(runPacket.playerid(), runPacket.b_run()));
			}
			break;
		}
		case 7: // Jump 메시지 타입 값
		{
			Protocol::jump jumpPacket;
			if (jumpPacket.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				Q_jump.push(PlayerJump(jumpPacket.playerid()));
			}
			break;
		}
		case 8: // ZombieDataList 메시지 타입 값
		{
			Protocol::ZombieDataList zombieDataList;
			if (zombieDataList.ParseFromArray(Packet.GetData(), Packet.Num()))
			{
				for (const auto& zombie : zombieDataList.zombies())
				{
					FVector NewLocation(zombie.x(), zombie.y(), zombie.z());
					FRotator NewRotation(zombie.pitch(), zombie.yaw(), zombie.roll());
					Q_zombie.push(ZombieData(zombie.zombieid(), NewLocation, NewRotation, zombie.zombietype()));
					UE_LOG(LogNet, Display, TEXT("ZombieData recv: %d"), zombie.zombieid());
				}
			}
			break;
		}
		default:
			UE_LOG(LogNet, Warning, TEXT("Unknown Packet Type: %d"), tempCharacterPacket.packet_type());
			break;
		}
	}
}


uint32 ClientSocket::GetMyPlayerId() const
{
	return MyPlayerId;
}

// RecvWorker implementation here (use FSocket* instead of SOCKET*)

RecvWorker::RecvWorker(FSocket* InSocket, TSharedPtr<PacketSession> Session)
	: Socket(InSocket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
}

RecvWorker::~RecvWorker()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill(true);
		delete Thread;
	}
}

bool RecvWorker::Init()
{
	UE_LOG(LogNet, Warning, TEXT("RecvWorker initialized"));
	return true;
}

uint32 RecvWorker::Run()
{
	while (Running)
	{
		TArray<uint8> Packet;

		if (ReceivePacket(OUT Packet))
		{
			if (SessionRef.IsValid())
			{
				SessionRef->RecvPacketQueue.Enqueue(Packet);
			}
		}
	}

	return 0;
}

void RecvWorker::Exit()
{
	Running = false;
}

void RecvWorker::Destroy()
{
	Running = false;
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OutPacket)
{
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize);

	if (!ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize))
		return false;

	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("Recv PacketID : %d, PacketSize : %d"), Header.PacketID, Header.PacketSize);
	}

	OutPacket = HeaderBuffer;

	const int32 PayloadSize = Header.PacketSize - HeaderSize;
	if (PayloadSize == 0)
		return true;

	OutPacket.AddZeroed(PayloadSize);

	return ReceiveDesiredBytes(&OutPacket[HeaderSize], PayloadSize);
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

// SendWorker implementation here (use FSocket* instead of SOCKET*)

SendWorker::SendWorker(FSocket* InSocket, TSharedPtr<PacketSession> Session)
	: Socket(InSocket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill(true);
		delete Thread;
	}
}

bool SendWorker::Init()
{
	UE_LOG(LogNet, Warning, TEXT("SendWorker initialized"));
	return true;
}

uint32 SendWorker::Run()
{
	while (Running)
	{
		SendBufferRef SendBuffer;

		if (SessionRef.IsValid())
		{
			if (SessionRef->SendPacketQueue.Dequeue(OUT SendBuffer))
			{
				SendPacket(SendBuffer);
			}
		}

		FPlatformProcess::Sleep(0.01f);
	}

	return 0;
}

void SendWorker::Exit()
{
	Running = false;
}

void SendWorker::Destroy()
{
	Running = false;
	if (Thread)
	{
		Thread->WaitForCompletion();
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	return SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->GetSize());
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (!Socket->Send(Buffer, Size, BytesSent))
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}

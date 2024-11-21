// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "ProGamemode/ProGameInstance.h"

ClientSocket::ClientSocket(UProGameInstance* Inst)
{
	gameInst = Inst;
	//CurrentServerType = ServerType::LOBBY_SERVER;
	CurrentServerType = ServerType::GAME_SERVER;

	recvBuffer.buf = recvData;
	recvBuffer.len = BUFSIZE;

	/*if (ConnectServer(ServerType::LOBBY_SERVER)) {
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	}*/

	if (ConnectServer(ServerType::GAME_SERVER)) {

		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	}
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
	UE_LOG(LogNet, Warning, TEXT("Thread has been initialized"));
	return true;
}

void ClientSocket::StartRecv()
{
	ZeroMemory(&recvOverlap, sizeof(recvOverlap));

	recvBuffer.len = BUFSIZE;
	recvBuffer.buf = reinterpret_cast<char*>(recvData);

	DWORD flags = 0;
	int result = WSARecv(Socket, &recvBuffer, 1, NULL, &flags, &recvOverlap, NULL);
	if (result == SOCKET_ERROR) {
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING) {
			UE_LOG(LogNet, Error, TEXT("WSARecv failed: %d"), err);
		}
	}
}

uint32 ClientSocket::Run()
{
	DWORD bytesTransferred;
	ULONG_PTR completionKey;
	LPOVERLAPPED overlapped = nullptr;

	while (true)
	{
		BOOL result = GetQueuedCompletionStatus(hIocp, &bytesTransferred, &completionKey, &overlapped, INFINITE);
		if (result == 0 || bytesTransferred == 0)
		{
			UE_LOG(LogNet, Warning, TEXT("Connection closed or error occurred"));
			return 0;
		}

		// 수신 완료된 데이터를 처리
		if (overlapped == &recvOverlap)
		{
			// lock-free 큐에 데이터 추가
			recvQueue.push(std::vector<char>(recvData, recvData + bytesTransferred));
			StartRecv();

			// 수신 큐 처리
			ProcessRecvQueue();
		}
	}
}

void ClientSocket::ProcessRecvQueue()
{
	std::vector<char> buffer;
	while (recvQueue.try_pop(buffer))
	{
		// 수신된 데이터 처리
		ProcessPacket(buffer);
	}
}


void ClientSocket::ProcessPacket(const std::vector<char>& buffer)
{
	if (!ReceivedPlayerId)
	{
		ReceivedPlayerId = true;

		memcpy(&MyPlayerId, buffer.data(), sizeof(MyPlayerId));

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MyPlayerId: %d"), MyPlayerId));
		UE_LOG(LogNet, Display, TEXT("Received MyPlayerId: %d"), MyPlayerId);

		return;
	}

	if (CurrentServerType == ServerType::LOBBY_SERVER) {
		Protocol::SC_Ready Packet;
		if (Packet.ParseFromArray(buffer.data(), buffer.size()))
		{
			switch (Packet.type())
			{
			case 3:
			{
				Protocol::SC_Login Login_Packet;
				if (Login_Packet.ParseFromArray(buffer.data(), buffer.size())) {
					b_login = Login_Packet.b_login();
					recv_login = true;
				}
				break;
			}
			case 4:
			{
				Protocol::SC_Register Register_Packet;
				if (Register_Packet.ParseFromArray(buffer.data(), buffer.size())) {
					b_register = Register_Packet.b_register();
					recv_register = true;
				}
				break;
			}

			case 6:
			{
				Protocol::SC_Ready Ready_Packet;
				if (Ready_Packet.ParseFromArray(buffer.data(), buffer.size())) {
					if (Ready_Packet.allready())
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
				}
				break;
			}

			case 7:
			{
				Protocol::Select_Character Select_Packet;

				if (Select_Packet.ParseFromArray(buffer.data(), buffer.size())) {
					Q_select.push(CharacterSelect(Select_Packet.playerid(), Select_Packet.character_type()));
				}

				UE_LOG(LogNet, Display, TEXT("Received Select Character: %d, %d"), Select_Packet.playerid(), Select_Packet.character_type());

				break;
			}

			case 9:
			{
				UE_LOG(LogNet, Display, TEXT("Received Join:"));


				break;
			}

			case 10:
			{
				Protocol::SC_JoinPlayer JP_Packet;

				if (JP_Packet.ParseFromArray(buffer.data(), buffer.size())) {
					Q_jplayer.push(JoinPlayer(JP_Packet.name(), JP_Packet.playerid()));
				}

				UE_LOG(LogNet, Display, TEXT("Received Join Player:"));

				break;
			}

			case 12:
			{
				UE_LOG(LogNet, Display, TEXT("Received Leave Player:"));

				break;
			}

			case 13:
			{
				UE_LOG(LogNet, Display, TEXT("Received Chatting:"));

				Protocol::SC_Chatting Chat_Packet;

				if (Chat_Packet.ParseFromArray(buffer.data(), buffer.size())) {

					FString FStringchat = FString(UTF8_TO_TCHAR(Chat_Packet.chat().c_str()));

					Q_chat.push(Chatting(Chat_Packet.playerid(), FStringchat));
				}

				break;
			}
			}
		}
	}


	else if (CurrentServerType == ServerType::GAME_SERVER) {
		Protocol::Character tempCharacterPacket;
		if (tempCharacterPacket.ParseFromArray(buffer.data(), buffer.size()))
		{
			// 메시지 타입 확인
			switch (tempCharacterPacket.packet_type())
			{
			case 1: // Character 메시지 타입 값
			{
				Protocol::Character CharacterPacket;
				if (CharacterPacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					OtherPlayerId = CharacterPacket.playerid();
					FVector NewLocation(CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
					FRotator NewRotation(CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

					if (OtherPlayerId != MyPlayerId) {
						Q_player.push(PlayerData(OtherPlayerId, NewLocation, NewRotation, CharacterPacket.charactertype(), CharacterPacket.hp()));
						Q_run.push(PlayerRun(CharacterPacket.playerid(), CharacterPacket.b_run()));
					}
				}
				break;
			}
			case 2: // Zombie 메시지 타입 값
			{
				Protocol::Zombie ZombiePacket;
				if (ZombiePacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					FVector NewLocation(ZombiePacket.x(), ZombiePacket.y(), ZombiePacket.z());
					FRotator NewRotation(ZombiePacket.pitch(), ZombiePacket.yaw(), ZombiePacket.roll());
					Q_zombie.push(ZombieData(ZombiePacket.zombieid(), NewLocation, NewRotation, ZombiePacket.zombietype()));
					//UE_LOG(LogNet, Display, TEXT("push Zombie: ZombieId=%d"), ZombiePacket.zombieid());
				}
				break;
			}
			case 3:
			{
				Protocol::Time TimePacket;
				if (TimePacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Timer = TimePacket.timer();
					//UE_LOG(LogNet, Display, TEXT("Timer: %d"), Timer);
				}
				break;
			}

			case 4:
			{
				Protocol::Character_Attack AttackPacket;
				if (AttackPacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_pattack.push(PlayerAttack(AttackPacket.playerid(), AttackPacket.attack()));
				}
				break;
			}

			case 5:
			{
				Protocol::Equip_Item EquipPacket;
				if (EquipPacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_eitem.push(EquipItem(EquipPacket.playerid(), EquipPacket.itemname(), EquipPacket.itemtype()));
				}
				break;
			}

			case 7:
			{
				Protocol::jump jumpPacket;
				if (jumpPacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_jump.push(PlayerJump(jumpPacket.playerid()));
				}
				break;
			}

			case 8:
			{
				Protocol::ZombieDataList zombieDataList;
				if (zombieDataList.ParseFromArray(buffer.data(), buffer.size()))
				{
					for (const auto& zombie : zombieDataList.zombies())
					{
						FVector NewLocation(zombie.x(), zombie.y(), zombie.z());
						FRotator NewRotation(zombie.pitch(), zombie.yaw(), zombie.roll());
						Q_zombie.push(ZombieData(zombie.zombieid(), NewLocation, NewRotation, zombie.zombietype()));
						UE_LOG(LogNet, Display, TEXT("ZombieSpawnData recv: %d, playerid = %d"), zombie.zombieid(), MyPlayerId);
					}
				}
				break;
			}
			case 10:
			{
				Protocol::ZombiePath zombiepath;
				if (zombiepath.ParseFromArray(buffer.data(), buffer.size()))
				{
					ZombiePath localZombiePath;

					localZombiePath.ZombieId = zombiepath.zombieid();
					//UE_LOG(LogNet, Display, TEXT("Zombie ID: #%d"), localZombiePath.ZombieId);

					// 경로 입력
					localZombiePath.Path1.emplace_back(zombiepath.path1().x(), zombiepath.path1().y(), zombiepath.path1().z());
					localZombiePath.Path2.emplace_back(zombiepath.path2().x(), zombiepath.path2().y(), zombiepath.path2().z());

					// 위치 값도 추가
					localZombiePath.Location = FVector(zombiepath.location().x(), zombiepath.location().y(), zombiepath.location().z());
					//UE_LOG(LogNet, Display, TEXT("Zombie #%d's Location: ( %.2f, %.2f, %.2f )"), localZombiePath.ZombieId, localZombiePath.Location.X, localZombiePath.Location.Y, localZombiePath.Location.Z);

					// 큐에 ZombiePath 객체를 추가
					Q_path.push(localZombiePath);
					//UE_LOG(LogNet, Display, TEXT("ZombiePath recv: %d"), localZombiePath.ZombieId);
				}
				break;
			}

			case 11:
			{
				Protocol::ping pingpacket;
				if (pingpacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_ping.push(true);
					//UE_LOG(LogNet, Display, TEXT("ping"));
				}
				break;
			}

			case 12:
			{
				Protocol::Zombie_hp zombiehp;
				if (zombiehp.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_zhp.push(ZombieHP(zombiehp.zombieid(), zombiehp.damage()));
					UE_LOG(LogNet, Display, TEXT("Zombie HP"));
				}
				break;
			}

			case 13:
			{
				Protocol::Zombie_attack zombieattack;
				if (zombieattack.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_zattack.push(ZombieAttack(zombieattack.zombieid(), zombieattack.playerid()));
					UE_LOG(LogNet, Display, TEXT("Zombie Attack"));
				}
				break;
			}

			case 15:
			{
				Protocol::ItemDataList itemdatalist;
				if (itemdatalist.ParseFromArray(buffer.data(), buffer.size()))
				{
					for (const auto& item : itemdatalist.items()) {
						Q_setitem.push(Set_Item(item.itemid(), item.itemname(), item.itemclass(), item.texture_path(),
							item.count(), item.floor(), FVector(item.posx(), item.posy(), item.posz())));
						//UE_LOG(LogNet, Display, TEXT("Set Itempacket"));
					}
				}
				break;
			}

			case 16:
			{
				Protocol::CarDataList cardatalist;
				if (cardatalist.ParseFromArray(buffer.data(), buffer.size()))
				{
					for (const auto& car : cardatalist.cars()) {
						Q_setcar.push(Set_Car(car.carid(), car.carname(), FVector(car.posx(), car.posy(), car.posz()), FRotator(car.pitch(), car.yaw(), car.roll()),
							car.carkeyname()));
						//UE_LOG(LogNet, Display, TEXT("Set carpacket"));
					}
				}
				break;
			}

			case 17:
			{
				Protocol::destroy_item destroyitem;
				if (destroyitem.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_ditem.push(Destroy_Item(destroyitem.itemid(), destroyitem.playerid()));
					UE_LOG(LogNet, Display, TEXT("Destroy item %d"), destroyitem.itemid());
				}
				break;
			}
			case 18:
			{
				Protocol::get_key getkey;
				if (getkey.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_getkey.push(Get_Key(getkey.itemid(), getkey.playerid()));
					UE_LOG(LogNet, Display, TEXT("get key %d"), getkey.itemid());
				}
				break;
			}
			case 19:
			{
				Protocol::escape escapepacket;
				if (escapepacket.ParseFromArray(buffer.data(), buffer.size()))
				{
					Q_escape.push(Escape_Root(escapepacket.playerid(), escapepacket.root()));
					UE_LOG(LogNet, Display, TEXT("escape root open %d"), escapepacket.root());
				}
				break;
			}
			}

		}
	}
}



void ClientSocket::Exit()
{
	if (Socket)
	{
		closesocket(Socket);
		WSACleanup();
		Socket = INVALID_SOCKET;
	}
}

bool ClientSocket::ConnectServer(ServerType serverType)
{
	ReceivedPlayerId = false;

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
	if (serverType == ServerType::LOBBY_SERVER)
	{
		ServerAddr.sin_port = htons(7777);
	}

	if (serverType == ServerType::GAME_SERVER)
	{
		ServerAddr.sin_port = htons(8777);
	}

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

		hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (hIocp == NULL)
		{
			UE_LOG(LogNet, Error, TEXT("Failed to create IOCP"));
			closesocket(Socket);
			WSACleanup();
			return false;
		}

		// 소켓을 IOCP에 연결
		if (CreateIoCompletionPort((HANDLE)Socket, hIocp, (ULONG_PTR)this, 0) == NULL)
		{
			UE_LOG(LogNet, Error, TEXT("Failed to associate socket with IOCP"));
			return false;
		}

		// 비동기 수신 시작
		StartRecv();
		return true;
	}
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(SendData);
	wsaBuf.len = SendSize;

	WSAOVERLAPPED sendOverlap = {};
	ZeroMemory(&sendOverlap, sizeof(sendOverlap));

	DWORD bytesSent = 0;
	int result = WSASend(Socket, &wsaBuf, 1, &bytesSent, 0, &sendOverlap, NULL);
	if (result == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING)
		{
			UE_LOG(LogNet, Error, TEXT("WSASend failed: %d"), err);
			return false;
		}
	}

	return true;
}

uint32 ClientSocket::GetMyPlayerId() const
{
	return MyPlayerId;
}
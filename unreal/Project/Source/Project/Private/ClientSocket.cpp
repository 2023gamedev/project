// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "ProGamemode/ProGameInstance.h"

ClientSocket::ClientSocket(UProGameInstance* Inst)
{
	gameInst = Inst;
	//CurrentServerType = ServerType::LOBBY_SERVER;
	CurrentServerType = ServerType::GAME_SERVER;

	//if (ConnectServer(ServerType::LOBBY_SERVER)) {
	//	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	//}

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

uint32 ClientSocket::Run()
{
	std::vector<char> buffer;

	while (1)
	{
		char tempBuff[BUFSIZE];
		int recvLen = recv(Socket, tempBuff, BUFSIZE, 0);

		if (!ReceivedPlayerId && recvLen > 0)
		{
			ReceivedPlayerId = true;

			memcpy(&MyPlayerId, tempBuff, sizeof(MyPlayerId));

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MyPlayerId: %d"), MyPlayerId));
			UE_LOG(LogNet, Display, TEXT("Received MyPlayerId: %d"), MyPlayerId);

			buffer.clear();

			continue;
		}

		else if (recvLen > 0)
		{

			buffer.insert(buffer.end(), tempBuff, tempBuff + recvLen);

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
					}
					buffer.clear();
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
							PlayerId = CharacterPacket.playerid();
							FVector NewLocation(CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
							FRotator NewRotation(CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

							if (PlayerId != MyPlayerId) {
								Q_player.push(PlayerData(PlayerId, NewLocation, NewRotation, CharacterPacket.charactertype(), CharacterPacket.hp()));
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

					case 6:
					{
						Protocol::run runPacket;
						if (runPacket.ParseFromArray(buffer.data(), buffer.size()))
						{
							Q_run.push(PlayerRun(runPacket.playerid(), runPacket.b_run()));
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
								UE_LOG(LogNet, Display, TEXT("ZombieData recv: %d, playerid = %d"), zombie.zombieid(), MyPlayerId);
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
							
							// =============================================================================== 이렇게 받으면 zombieid 제대로 뽑는게 맞는지 확인 => case 8번에서는 id도 for문을 돌려서 뽑음
							localZombiePath.ZombieId = zombiepath.zombieid();
							UE_LOG(LogNet, Display, TEXT("Zombie ID: #%d"), zombiepath.zombieid());
							// ===============================================================================

							// 프로토콜 버퍼의 path 값을 localZombiePath의 Path 벡터에 추가
							for (const auto& path : zombiepath.path())
							{
								// Vector3를 std::tuple<float, float, float>로 변환하여 추가
								localZombiePath.Path.emplace_back(path.x(), path.y(), path.z());
							}

							// 위치 값도 추가
							localZombiePath.Location = FVector(zombiepath.location().x(), zombiepath.location().y(), zombiepath.location().z());

							// 큐에 ZombiePath 객체를 추가
							Q_path.push(localZombiePath);
							UE_LOG(LogNet, Display, TEXT("ZombiePath recv: %d"), localZombiePath.ZombieId);
						}
						break;
					}

					case 11:
					{
						Protocol::ping pingpacket;
						if (pingpacket.ParseFromArray(buffer.data(), buffer.size()))
						{
							Q_ping.push(true);
							UE_LOG(LogNet, Display, TEXT("ping"));
						}
						break;
					}

					case 12:
					{
						Protocol::Zombie_hp zombiehp;
						if (zombiehp.ParseFromArray(buffer.data(), buffer.size()))
						{
							Q_zhp.push(ZombieHP(zombiehp.zombieid(), zombiehp.hp()));
							UE_LOG(LogNet, Display, TEXT("Zombie HP"));
						}
						break;
					}

					case 13:
					{
						Protocol::Zombie_attack zombieattack;
						if (zombieattack.ParseFromArray(buffer.data(), buffer.size()))
						{
							Q_zattack.push(zombieattack.zombieid());
							UE_LOG(LogNet, Display, TEXT("Zombie Attack"));
						}
						break;
					}

					buffer.clear();
					}
				}
			}
		}
		else if (recvLen == 0)
		{
			UE_LOG(LogNet, Warning, TEXT("Connection closed"));
			return 0;
		}
		else
		{
			UE_LOG(LogNet, Warning, TEXT("Recv Error"));
			return 0;
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
		ServerAddr.sin_port = htons(8888);
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
		return true;
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
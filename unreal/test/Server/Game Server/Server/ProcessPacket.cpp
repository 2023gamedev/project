#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "algorithm"
#include "ZombieController.h"
#include "Zombie.h"

bool IOCP_CORE::IOCP_ProcessPacket(int id, Packet* buffer, int bufferSize) {
    // g_players에서 클라이언트 정보 검색
    auto it = g_players.find(id);
    if (it == g_players.end()) {
        // 유효하지 않은 클라이언트 ID에 대한 처리
        return false;
    }

    auto min_it = std::min_element(g_players.begin(), g_players.end(),
        [](const std::pair<unsigned int, PLAYER_INFO*>& a, const std::pair<unsigned int, PLAYER_INFO*>& b) {
            return a.first < b.first;
        });

    PLAYER_INFO* clientInfo = it->second;

    Protocol::Character tempPacket;
    tempPacket.ParseFromArray(buffer, bufferSize);

    if ((tempPacket.playerid() == id || tempPacket.isingame()) && !clientInfo->isInGame) {
        clientInfo->isInGame = true;
        b_Timer = true;
        zombieclass->SendZombieData(id);
        printf("SendZombieData!! Playerid=#%d\n", id);
    }

    // 패킷의 타입을 확인하여 처리
    switch (tempPacket.packet_type()) {
    case 1: {
        //printf("\n[ No. %3u ] character Packet Received !!\n", id);
        //printf("Received packet type = %d\n", CharacterPacket.type());
        //printf("Received playerID = %d\n", CharacterPacket.playerid());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

        // 서버로 받은 패킷을 그대로 돌려줌
        Protocol::Character Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        Player pl;
        pl.x = Packet.x();
        pl.y = Packet.y();
        pl.z = Packet.z();

        pl.health = Packet.hp();

        if (pl.z < 800.f) {
            pl.floor = FLOOR::FLOOR_B2;
        }
        else if (pl.z < 1800.f) {
            pl.floor = FLOOR::FLOOR_B1;
        }
        else if (pl.z < 2500.f) {
            pl.floor = FLOOR::FLOOR_F1;
        }
        else if (pl.z < 3600.f) {
            pl.floor = FLOOR::FLOOR_F2;
        }
        else {
            pl.floor = FLOOR::FLOOR_F3;
        }

        // 지금은 수정 됐지만 혹시해서 남김 -> 클라 플레이어 초기화 id 설정값이 99인데 이걸 전송 받는 경우가 생겼었다
        if (Packet.playerid() != 99) {
            playerDB[Packet.playerid()] = pl;
        }

        cout << boolalpha << Packet.b_run() << endl;

        for (auto& player : playerDB) {
            if (player.first == Packet.playerid()) {
                if (Packet.b_run() == 1) {
                    player.second.IsRunning = false;
                }
                else if (Packet.b_run() == 2) {
                    player.second.IsRunning = true;
                }
            }
        }

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    case 2: {
        // 예전에 클라에서 좀비 움직이면 해당 패킷을 서버로 보냈음 -> 이제는 움직임을 서버에서 담당하니 사실상 사용 안 함

        //printf("\n[ No. %3u ] zombie Packet Received !!\n", id);
        Protocol::Zombie Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        /*ZombieData zombiedata;
        zombiedata.zombieID = Packet.zombieid();
        zombiedata.x = Packet.x();
        zombiedata.y = Packet.y();
        zombiedata.z = Packet.z();
        zombiedata.pitch = Packet.pitch();
        zombiedata.yaw = Packet.yaw();
        zombiedata.roll = Packet.roll();*/

        //ZombieController zombiecontroller;

        //printf("zombie id: %d \n", Packet.zombieid());

        /*if (find(m_zombie.begin(), m_zombie.end(), zombiedata.zombieID) == m_zombie.end()) {
            zombiecontroller.addZombie(zombiedata);
            m_zombie.push_back(zombiedata.zombieID);
        }
        else {
            zombiecontroller.setZombiePosition(zombiedata);
        }*/

        if (id == min_it->first) {
            for (const auto& player : g_players) {
                if (player.first != id && player.second->isInGame) {
                    IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                }
            }
        }
        return true;
    } break;

    case 4:
    {
        printf("\n[ No. %3u ] character Packet Received !!\n", id);
        Protocol::Character_Attack Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    }

    case 5:
    {
        printf("\n[ No. %3u ] Equip Packet Received !!\n", id);
        Protocol::Equip_Item Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    }
    //case 6:
    //{
    //    printf("\n[ No. %3u ] Run Packet Received !!\n", id);
    //    Protocol::run Packet;
    //    Packet.ParseFromArray(buffer, bufferSize);
    //    string serializedData;
    //    Packet.SerializeToString(&serializedData);

    //    cout << boolalpha << Packet.b_run() << endl;  // -> protobuf bool값 잘 받는 지 확인 -> 잘,,, 받네?? (false는 직렬화 안한다고 들었는데;;) 
    //    
    //    // 해당 플레이어 run-bool값 변경
    //    for (auto& player : playerDB) {
    //        if (player.first == Packet.playerid()) {
    //            player.second.IsRunning = Packet.b_run();
    //        }
    //    }

    //    // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
    //    for (const auto& player : g_players) {
    //        if (player.first != id && player.second->isInGame) {
    //            IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
    //        }
    //    }
    //    return true;
    //}

    case 7:
    {
        printf("\n[ No. %3u ] Jump Packet Received !!\n", id);
        Protocol::jump Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    }

    case 9:
    {
        printf("\n[ No. %3u ] Detected Packet Received !!\n", id);
        Protocol::Detected Packet;
        Packet.ParseFromArray(buffer, bufferSize);

        int recvzombieid = Packet.zombieid();

        if (Packet.player_insight()) {
            for (auto& z : zombie) {
                if (z->ZombieData.zombieID == recvzombieid) {
                    z->PlayerInSight = true;
                    z->KnewPlayerLocation = true;
                    z->SetDistance(Packet.playerid());   //DistanceToPlayers 맵 에 해당 플레이어와 거리 추가하기

                    //cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 의 시야에 - 플레이어 \'#" << id << "\' 포착!!!: " << endl;

                    break;
                }
            }
        }

        else {
            for (auto& z : zombie) {
                if (z->ZombieData.zombieID == recvzombieid) {
                    z->PlayerInSight = false;
                    //z->DistanceToPlayers.erase(Packet.playerid());   //DistanceToPlayers 맵 에 해당 플레이어 정보 삭제하기 [X]"데이터 레이스!!"-> BT 쓰레드에서 at 사용 많이해서 abort 크래쉬 생김
                    //z->DistanceToPlayers.at(Packet.playerid()) = -1.0f;
                    z->DistanceToPlayers[Packet.playerid()] = -1.0f;     //그냥 이렇게 마이너스값 넣어 놓고 이건 없는 데이터로 치자 (마이너스값은 절대 설정될 수 없으니 & )

                    //cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 의 시야에서 - 플레이어 \'#" << id << "\' 놓침!!!: " << endl;

                    break;
                }
            }
        }

        return true;
    }

    case 11:
    {
        Protocol::ping Packet;
        Packet.ParseFromArray(buffer, bufferSize);

        auto it = g_players.find(Packet.playerid());

        if (it != g_players.end())
        {
            PLAYER_INFO* player = it->second;
            player->pingcnt = 0; // 클라이언트가 응답했으므로 pingcnt 초기화
        }

        return true;
    }

    case 12:
    {
        Protocol::Zombie_hp Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        cout << "recv zombie #" << Packet.zombieid() << " HP Packet" << endl;

        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 다른 플레이어들에게 그대로 전송 
        /*for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }*/

        // 중복 방지로 클라이언트 하나의 정보만 동기화
        if (id == min_it->first) {
            for (const auto& player : g_players) {
                if (player.first != id && player.second->isInGame) {
                    IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                }
            }
        }

        // 좀비 HP 업데이트
        int recvzombieid = Packet.zombieid();

        for (auto& z : zombie) {
            if (z->ZombieData.zombieID == recvzombieid) {
                z->zombieHP = Packet.hp();

                if (z->ZombieData.zombietype == 0 && z->zombieHP < z->NormalZombieStartHP) {
                    z->IsBeingAttacked = true;  // 좀비 피격중으로 변경
                    z->HaveToWait = true;	// 좀비 BT 대기상태로 변경
                    z->animStartTime = std::chrono::high_resolution_clock::now();		// 좀비 피격 시작 시간

                    cout << "================================================================================================================================================================================" << endl;
                    cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 피격!! 남은 HP: " << z->GetHP() << endl;
                    cout << "================================================================================================================================================================================" << endl;
                }
            }
        }

        return true;
    }

    case 14:
    {
        Protocol::patrol_hit Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        cout << "recv patrol zombie hit" << Packet.zombieid() << endl;

        return true;
    }

    default: {
        printf("\nERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.packet_type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
        return true;
    } break;
    }
}


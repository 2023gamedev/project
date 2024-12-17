#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "algorithm"
#include "ZombieController.h"
#include "ItemController.h"
#include "Zombie.h"
#include "ShoutingZombie.h"

#include <algorithm>

bool IOCP_CORE::IOCP_ProcessPacket(int id, const std::string &packet) {
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
    tempPacket.ParseFromArray(packet.data(), packet.size());

    if ((tempPacket.playerid() == id || tempPacket.isingame()) && !clientInfo->isInGame) {
        clientInfo->isInGame = true;
        b_Timer = true;
        //printf("SendDatas!! Playerid=#%d\n", id);
    }

    if (!clientInfo->send_zombie) {
        zombieclass->SendZombieData(id);
    }

    if (clientInfo->send_zombie && !clientInfo->send_item) {
        itemclass->SendItemData(id);
    }

    if (clientInfo->send_zombie && clientInfo->send_item && !clientInfo->send_car) {
        itemclass->SendCarData(id);
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
        Packet.ParseFromArray(packet.data(), packet.size());
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 지금은 수정 됐지만 혹시해서 남김 -> 클라 플레이어 초기화 id 설정값이 99인데 이걸 전송 받는 경우가 생겼었다
        if (Packet.playerid() != 99) {
            //playerDB[Packet.playerid()] = pl; //-> 이렇게 사용하면 초기화 빼먹은 값 더미 값 씌워질 수 있음
            playerDB[Packet.playerid()].username = Packet.username();

            playerDB[Packet.playerid()].x = Packet.x();
            playerDB[Packet.playerid()].y = Packet.y();
            playerDB[Packet.playerid()].z = Packet.z();

            playerDB[Packet.playerid()].health = Packet.hp();

            //cout << "player hp: " << Packet.hp() << endl;

            if (Packet.b_run() == 1) {
                playerDB[Packet.playerid()].IsRunning = false;
                //printf("\n[ No. %3u ] Walk Packet Received !!\n", id);
            }
            else if (Packet.b_run() == 2) {
                playerDB[Packet.playerid()].IsRunning = true;
                //printf("\n[ No. %3u ] Run Packet Received !!\n", id);
            }

            //cout << "player IsRunning: " << Packet.b_run() << endl;

            if (Packet.z() < 800.f) {
                playerDB[Packet.playerid()].floor = FLOOR::FLOOR_B2;
            }
            else if (Packet.z() < 1800.f) {
                playerDB[Packet.playerid()].floor = FLOOR::FLOOR_B1;
            }
            else if (Packet.z() < 2500.f) {
                playerDB[Packet.playerid()].floor = FLOOR::FLOOR_F1;
            }
            else if (Packet.z() < 3600.f) {
                playerDB[Packet.playerid()].floor = FLOOR::FLOOR_F2;
            }
            else {
                playerDB[Packet.playerid()].floor = FLOOR::FLOOR_F3;
            }
        }

        bool bAllPlayersInRange = true;
        int alive_cnt = 0;
        int dead_cnt = 0;
        int bestkill_cnt = 0;
        std::string bestkill_player;

        if (b_IsEscaping) {
            for (const auto& player : playerDB) {
                if (player.second.health != 0) {
                    alive_cnt++;
                    float DeltaX = std::abs(player.second.x - Escape_Location.x);
                    float DeltaY = std::abs(player.second.y - Escape_Location.y);

                    // 범위를 벗어나는지 확인
                    if (DeltaX >= 100.0f || DeltaY >= 100.0f || player.second.floor != Escape_Location.floor) {
                        bAllPlayersInRange = false; // 한 명이라도 범위를 벗어나면 false
                        break;
                    }
                }
                else {
                    dead_cnt++;
                }

                if (bestkill_cnt < player.second.killcount) {
                    bestkill_cnt = player.second.killcount;
                    bestkill_player = player.second.username;
                }
            }

            if (bAllPlayersInRange) {
                Protocol::game_clear clear_packet;

                clear_packet.set_packet_type(20);
                clear_packet.set_root(Escape_Root);
                clear_packet.set_alive_players(alive_cnt);
                clear_packet.set_dead_players(dead_cnt);
                clear_packet.set_open_player(Root_Open_Player);
                
                clear_packet.set_best_killcount(bestkill_cnt);
                clear_packet.set_best_kill_player(bestkill_player);

                for (const auto& player : g_players) {
                    if (player.second->isInGame) {

                        clear_packet.set_my_killcount(playerDB[player.first].killcount);

                        string serializedData;
                        clear_packet.SerializeToString(&serializedData);
                        IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                    }
                }

                return true;
            }
        }

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame && player.second != nullptr) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    case 2: {
        // 예전에 클라에서 좀비 움직이면 해당 패킷을 서버로 보냈음 -> 이제는 움직임을 서버에서 담당하니 사실상 사용 안 함

        printf("\n[ No. %3u ] zombie Packet Received !!\n", id);
        Protocol::Zombie Packet;
        Packet.ParseFromArray(packet.data(), packet.size());
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
        Packet.ParseFromArray(packet.data(), packet.size());
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
        Packet.ParseFromArray(packet.data(), packet.size());
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
    //    Packet.ParseFromArray(packet.data(), packet.size());
    //    string serializedData;
    //    Packet.SerializeToString(&serializedData);

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
        Packet.ParseFromArray(packet.data(), packet.size());
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
        Packet.ParseFromArray(packet.data(), packet.size());

        int recvzombieid = Packet.zombieid();

        if (Packet.player_insight()) {
            for (auto& z : zombieDB) {
                if (z->ZombieData.zombieID == recvzombieid) {
                    if (z->IsAttacking)
                        break;

                    z->PlayerInSight = true;
                    z->KnewPlayerLocation = true;
                    z->SetDistance(Packet.playerid(), 1, 1);   //DistanceTo_PlayerInsight 맵 에 해당 플레이어와 거리 추가하기

#ifdef ENABLE_BT_LOG
                    cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 의 시야에 - 플레이어 \'#" << id << "\' 포착!!!: " << endl;
#endif

                    // 샤우팅 좀비일 경우에
                    if (z->ZombieData.zombietype == 1) {
                        ShoutingZombie* sz = dynamic_cast<ShoutingZombie*>(z);  // 다운 캐스팅 사용!
                        sz->Shout(zombieDB);
                    }

                    break;
                }
            }
        }

        else {
            for (auto& z : zombieDB) {
                if (z->ZombieData.zombieID == recvzombieid) {
                    if (z->IsAttacking)
                        break;

                    z->PlayerInSight = false;
                    z->DistanceTo_PlayerInsight[Packet.playerid()] = -1.0f;     //그냥 이렇게 마이너스값 넣어 놓고 이건 없는 데이터로 치자 (마이너스값은 절대 설정될 수 없으니)

#ifdef ENABLE_BT_LOG
                    cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 의 시야에서 - 플레이어 \'#" << id << "\' 놓침!!!: " << endl;
#endif

                    break;
                }
            }
        }

        return true;
    }

    case 11:
    {
        Protocol::ping Packet;
        Packet.ParseFromArray(packet.data(), packet.size());

        auto it = g_players.find(Packet.playerid());

        if (it != g_players.end() && it->second != nullptr)
        {
            PLAYER_INFO* player = it->second;
            player->pingcnt = 0; // 클라이언트가 응답했으므로 pingcnt 초기화
        }

        return true;
    }

    case 12:
    {
        Protocol::Zombie_hp Packet;
        Packet.ParseFromArray(packet.data(), packet.size());
        cout << "recv zombie #" << Packet.zombieid() << " HP Packet" << endl;

        string serializedData;
        Packet.SerializeToString(&serializedData);

        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }

        // 좀비 HP 업데이트
        int recvzombieid = Packet.zombieid();

        for (auto& z : zombieDB) {
            if (z->ZombieData.zombieID == recvzombieid) {
                z->zombieHP = max(0, z->zombieHP - Packet.damage());

                if (z->zombieHP < z->NormalZombieStartHP) {
                    z->IsBeingAttacked = true;  // 좀비 피격중으로 변경
                    z->HaveToWait = true;	// 좀비 BT 대기상태로 변경
                    z->animStartTime = std::chrono::high_resolution_clock::now();		// 좀비 피격 시작 시간

                    //cout << "================================================================================================================================================================================" << endl;
                    cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 피격!! 남은 HP: " << z->GetHP() << endl;
                    cout << endl;
                    //cout << "================================================================================================================================================================================" << endl;
                }

                if (z->zombieHP <= 0) {
                    playerDB[id].killcount++;

                    cout << "좀비 \'#" << z->ZombieData.zombieID << "\' 사망!!!" << endl;
                    cout << endl;
                }
            }
        }

        return true;
    }

    case 14:
    {
        Protocol::patrol_hit Packet;
        Packet.ParseFromArray(packet.data(), packet.size());
        //cout << "recv patrol zombie hit #" << Packet.zombieid() << endl;

        return true;
    }

    case 17:
    {
        printf("\n[ No. %3u ] Destroy Item Packet Received !!\n", id);
        Protocol::destroy_item Packet;
        Packet.ParseFromArray(packet.data(), packet.size());
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                printf("%d 한테 키 보냈음\n", player.first);
            }
        }
        return true;
    }

    case 18:
    {
        printf("\n[ No. %3u ] get key item !!\n", id);
        Protocol::get_key Packet;
        Packet.ParseFromArray(packet.data(), packet.size());

        int itemboxid = Packet.itemboxid();
        int playerid = Packet.playerid();

        printf("%d item\n", Packet.itemid());

        string serializedData;
        Packet.SerializeToString(&serializedData);

        for (const auto& player : g_players) {
            if (player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                printf("%d 한테 키 보냈음\n", player.first);
            }
        }

        //key 먹었을 경우에는 아이템박스 삭제 패킷을 여기서 따로 보내줌

        Protocol::destroy_item destroyPacket;

        destroyPacket.set_packet_type(17);
        destroyPacket.set_itemid(itemboxid);
        destroyPacket.set_playerid(playerid);

        string DserializedData;
        destroyPacket.SerializeToString(&DserializedData);

        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, DserializedData.data(), DserializedData.size());
                printf("%d 한테 키 삭제 보냈음\n", player.first);
            }
        }

        return true;
    }

    case 19:
    {
        printf("\n[ No. %3u ] escape root open !!\n", id);
        Protocol::escape Packet;
        Packet.ParseFromArray(packet.data(), packet.size());
        string serializedData;
        Packet.SerializeToString(&serializedData);

        if (Packet.root() == 2) {
            roofkey_cnt++;
        }

        if (roofkey_cnt == 2 || Packet.root() == 1) {
            // 문 연 위치 저장, 상태를 탈출 준비 상태로 변경
            Escape_Location.x = playerDB[id].x;
            Escape_Location.y = playerDB[id].y;
            Escape_Location.floor = playerDB[id].floor;

            b_IsEscaping = true;
            Escape_Root = Packet.root();
            Root_Open_Player = playerDB[id].username;
        }


        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            if (player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    }

    case 21:
    {
        printf("\n[ No. %3u ] recv send complet packet !!\n", id);
        Protocol::send_complete Packet;
        Packet.ParseFromArray(packet.data(), packet.size());

        if (Packet.complete_type() == 1) {
            clientInfo->send_zombie = true;
        }
        else if (Packet.complete_type() == 2) {
            clientInfo->send_item = true;
        }
        else if (Packet.complete_type() == 3) {
            clientInfo->send_car = true;
        }

        return true;
    }

    default: {
        printf("\nERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.packet_type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
        return true;
    } break;
    }
}


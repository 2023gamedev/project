#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "algorithm"
#include "ZombieController.h"
#include "Zombie.h"

bool IOCP_CORE::IOCP_ProcessPacket(int id, Packet* buffer, int bufferSize) {
    // g_players���� Ŭ���̾�Ʈ ���� �˻�
    auto it = g_players.find(id);
    if (it == g_players.end()) {
        // ��ȿ���� ���� Ŭ���̾�Ʈ ID�� ���� ó��
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

    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (tempPacket.packet_type()) {
    case 1: {
        //printf("\n[ No. %3u ] character Packet Received !!\n", id);
        //printf("Received packet type = %d\n", CharacterPacket.type());
        //printf("Received playerID = %d\n", CharacterPacket.playerid());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

        // ������ ���� ��Ŷ�� �״�� ������
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

        // ������ ���� ������ Ȥ���ؼ� ���� -> Ŭ�� �÷��̾� �ʱ�ȭ id �������� 99�ε� �̰� ���� �޴� ��찡 �������
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

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
        for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    case 2: {
        // ������ Ŭ�󿡼� ���� �����̸� �ش� ��Ŷ�� ������ ������ -> ������ �������� �������� ����ϴ� ��ǻ� ��� �� ��

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

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
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

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
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

    //    cout << boolalpha << Packet.b_run() << endl;  // -> protobuf bool�� �� �޴� �� Ȯ�� -> ��,,, �޳�?? (false�� ����ȭ ���Ѵٰ� ����µ�;;) 
    //    
    //    // �ش� �÷��̾� run-bool�� ����
    //    for (auto& player : playerDB) {
    //        if (player.first == Packet.playerid()) {
    //            player.second.IsRunning = Packet.b_run();
    //        }
    //    }

    //    // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
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

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
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
                    z->SetDistance(Packet.playerid());   //DistanceToPlayers �� �� �ش� �÷��̾�� �Ÿ� �߰��ϱ�

                    //cout << "���� \'#" << z->ZombieData.zombieID << "\' �� �þ߿� - �÷��̾� \'#" << id << "\' ����!!!: " << endl;

                    break;
                }
            }
        }

        else {
            for (auto& z : zombie) {
                if (z->ZombieData.zombieID == recvzombieid) {
                    z->PlayerInSight = false;
                    //z->DistanceToPlayers.erase(Packet.playerid());   //DistanceToPlayers �� �� �ش� �÷��̾� ���� �����ϱ� [X]"������ ���̽�!!"-> BT �����忡�� at ��� �����ؼ� abort ũ���� ����
                    //z->DistanceToPlayers.at(Packet.playerid()) = -1.0f;
                    z->DistanceToPlayers[Packet.playerid()] = -1.0f;     //�׳� �̷��� ���̳ʽ��� �־� ���� �̰� ���� �����ͷ� ġ�� (���̳ʽ����� ���� ������ �� ������ & )

                    //cout << "���� \'#" << z->ZombieData.zombieID << "\' �� �þ߿��� - �÷��̾� \'#" << id << "\' ��ħ!!!: " << endl;

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
            player->pingcnt = 0; // Ŭ���̾�Ʈ�� ���������Ƿ� pingcnt �ʱ�ȭ
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

        // �ٸ� �÷��̾�鿡�� �״�� ���� 
        /*for (const auto& player : g_players) {
            if (player.first != id && player.second->isInGame) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }*/

        // �ߺ� ������ Ŭ���̾�Ʈ �ϳ��� ������ ����ȭ
        if (id == min_it->first) {
            for (const auto& player : g_players) {
                if (player.first != id && player.second->isInGame) {
                    IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                }
            }
        }

        // ���� HP ������Ʈ
        int recvzombieid = Packet.zombieid();

        for (auto& z : zombie) {
            if (z->ZombieData.zombieID == recvzombieid) {
                z->zombieHP = Packet.hp();

                if (z->ZombieData.zombietype == 0 && z->zombieHP < z->NormalZombieStartHP) {
                    z->IsBeingAttacked = true;  // ���� �ǰ������� ����
                    z->HaveToWait = true;	// ���� BT �����·� ����
                    z->animStartTime = std::chrono::high_resolution_clock::now();		// ���� �ǰ� ���� �ð�

                    cout << "================================================================================================================================================================================" << endl;
                    cout << "���� \'#" << z->ZombieData.zombieID << "\' �ǰ�!! ���� HP: " << z->GetHP() << endl;
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
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
        return true;
    } break;
    }
}


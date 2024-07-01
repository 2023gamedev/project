#pragma once
#include "Common.h"
#include "algorithm"
#include "ZombieController.h"

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

    if (tempPacket.playerid() == id || tempPacket.isingame()) {
        clientInfo->isInGame = true;
        b_Timer = true;
    }

    // 패킷의 타입을 확인하여 처리
    switch (tempPacket.packet_type()) {
    case 1: {
        printf("[ No. %3u ] character Packet Received !!\n", id);
        //printf("Received packet type = %d\n", CharacterPacket.type());
        //printf("Received playerID = %d\n", CharacterPacket.playerid());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

        // 서버로 받은 패킷을 그대로 돌려줌
        Protocol::Character Packet;
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
    } break;

    case 2: {
        //printf("[ No. %3u ] zombie Packet Received !!\n", id);
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
        printf("[ No. %3u ] character Packet Received !!\n", id);
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
        printf("[ No. %3u ] Equip Packet Received !!\n", id);
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
    case 6:
    {
        printf("[ No. %3u ] Run Packet Received !!\n", id);
        Protocol::run Packet;
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

    case 7:
    {
        printf("[ No. %3u ] Jump Packet Received !!\n", id);
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

    default: {
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.packet_type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
        return true;
    } break;
    }
}


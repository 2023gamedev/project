#pragma once
#include "Common.h"
#include "algorithm"

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

    Protocol::CS_Login tempPacket;
    tempPacket.ParseFromArray(buffer, bufferSize);

    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (tempPacket.type()) {
    case 1: {
        printf("[ No. %3u ] Login Packet Received !!\n", id);

        Protocol::CS_Login CS_Packet;

        CS_Packet.ParseFromArray(buffer, bufferSize);

        Protocol::SC_Login SC_Packet;

        bool b_login = loginmanager.Login(CS_Packet.id(), CS_Packet.password());

        SC_Packet.set_type(3);
        SC_Packet.set_b_login(b_login);

        if (b_login) {
            g_players[id]->username = CS_Packet.id();
        }

        string serializedData;
        SC_Packet.SerializeToString(&serializedData);

        IOCP_SendPacket(id, serializedData.data(), serializedData.size());

        return true;

    }

    case 2: {
        printf("[ No. %3u ] Register Packet Received !!\n", id);
        Protocol::CS_Register CS_Packet;
        CS_Packet.ParseFromArray(buffer, bufferSize);

        Protocol::SC_Register SC_Packet;

        SC_Packet.set_type(4);
        SC_Packet.set_b_register(loginmanager.RegisterName(CS_Packet.id(), CS_Packet.password()));

        string serializedData;
        SC_Packet.SerializeToString(&serializedData);

        IOCP_SendPacket(id, serializedData.data(), serializedData.size());

        return true;

    }

    case 5: {
        printf("[ No. %3u ] Ready Packet Received !!\n", id);

        Protocol::CS_Ready CS_Packet;
        CS_Packet.ParseFromArray(buffer, bufferSize);

        g_players[CS_Packet.playerid()]->ready = CS_Packet.ready();

        int ready_cnt = 0;
        for (const auto& player : g_players) {
            if (player.second->room_num == g_players[CS_Packet.playerid()]->room_num
                && player.second->ready == true) {
                ready_cnt++;
            }
        }

        if (ready_cnt == 2) {
            Protocol::SC_WaitingReady SC_Packet;
            SC_Packet.set_type(14);
            SC_Packet.set_allready(true);

            std::string serializedData;
            SC_Packet.SerializeToString(&serializedData);

            for (const auto& player : g_players) {
                if (player.second->room_num == g_players[CS_Packet.playerid()]->room_num) {
                    IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                    g_players[player.first]->ready = false;
                }
            }
        }

        return true;

    }

    case 7:
    {
        printf("[ No. %3u ] Select Packet Received !!\n", id);

        Protocol::Select_Character Select_Packet;
        Select_Packet.ParseFromArray(buffer, bufferSize);
        cout << Select_Packet.character_type() << endl;

        string serializedData;
        Select_Packet.SerializeToString(&serializedData);

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
        for (const auto& player : g_players) {
            IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
        return true;
    }

    case 8:
    {
        printf("[ No. %3u ] Join Packet Received !!\n", id);

        Protocol::CS_Join CS_Packet;
        CS_Packet.ParseFromArray(buffer, bufferSize);
        
        int room_id = CS_Packet.roomid();
        int player_id = CS_Packet.playerid();

        Room* room = nullptr;

        auto it = rooms.find(room_id);
        if (it != rooms.end()) {
            room = it->second;
        }

        if (room->JoinRoom()) {
            g_players[id]->room_num = room_id;

            // ���� ���� �÷��̾�� ���� ���� �÷��̾� ���� ����
            for (const auto& player : g_players) {
                if (player.second->room_num == room_id && player.first != id) {
                    Protocol::SC_JoinPlayer ExistingPlayerPacket;

                    ExistingPlayerPacket.set_name(player.second->username); // ���� �÷��̾� ����
                    ExistingPlayerPacket.set_playerid(player.first);
                    ExistingPlayerPacket.set_type(10);

                    string existingSerializedData;
                    ExistingPlayerPacket.SerializeToString(&existingSerializedData);

                    // ���� ���� �÷��̾�� ���� �÷��̾� ���� ����
                    IOCP_SendPacket(id, existingSerializedData.data(), existingSerializedData.size());
                }
            }

            Protocol::SC_JoinPlayer SC_Packet;
            SC_Packet.set_name(g_players[id]->username); // ���� ���� �÷��̾� ����
            SC_Packet.set_playerid(id);
            SC_Packet.set_type(10);
            string serializedData;
            SC_Packet.SerializeToString(&serializedData);

            for (const auto& player : g_players) {
                if (player.second->room_num == room_id) {
                    // �� �÷��̾� ���� ��ε�ĳ��Ʈ
                    IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
                }
            }
            return true;
        }

        else {
            Protocol::SC_Join SC_Packet;

            SC_Packet.set_b_join(false);
            SC_Packet.set_type(9);
            string serializedData;
            SC_Packet.SerializeToString(&serializedData);
        }

        printf("Player %d : join room %d", id, CS_Packet.roomid());

        return true;
    }

    case 11:
    {
        printf("[ No. %3u ] Leave Packet Received !!\n", id);

        Protocol::CS_Leave CS_Packet;
        CS_Packet.ParseFromArray(buffer, bufferSize);

        int player_id = CS_Packet.playerid();
        int room_id = g_players[player_id]->room_num;

        auto it = rooms.find(room_id);
        Room* room = it->second;

        room->LeaveRoom();

        Protocol::SC_LeavePlayer SC_Packet;
        SC_Packet.set_username(g_players[id]->username);
        SC_Packet.set_playerid(player_id);
        SC_Packet.set_type(12);
        string serializedData;
        SC_Packet.SerializeToString(&serializedData);

        for (const auto& player : g_players) {
            if (player.second->room_num == room_id && player.first != id) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }

        g_players[player_id]->room_num = 0;

        return true;
    }

    case 13:
    {
        printf("[ No. %3u ] Chatting Packet Received !!\n", id);

        Protocol::CS_Chatting Packet;
        Packet.ParseFromArray(buffer, bufferSize);

        string serializedData;
        Packet.SerializeToString(&serializedData);

        int room_num = g_players[id]->room_num;

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
        for (const auto& player : g_players) {
            if (player.second->room_num == room_num) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    }


    default: {
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.type());
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
        return true;
    } break;
    }
}


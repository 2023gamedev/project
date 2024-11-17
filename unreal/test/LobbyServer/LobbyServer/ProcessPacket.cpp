#pragma once
#include "Common.h"
#include "algorithm"

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

    Protocol::CS_Login tempPacket;
    tempPacket.ParseFromArray(buffer, bufferSize);

    // 패킷의 타입을 확인하여 처리
    switch (tempPacket.type()) {
    case 1: {
        printf("[ No. %3u ] Login Packet Received !!\n", id);

        Protocol::CS_Login CS_Packet;

        CS_Packet.ParseFromArray(buffer, bufferSize);

        Protocol::SC_Login SC_Packet;

        SC_Packet.set_type(3);
        SC_Packet.set_b_login(loginmanager.Login(CS_Packet.id(), CS_Packet.password()));

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

        players_ready[CS_Packet.playerid()] = CS_Packet.ready();

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

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
        return true;
    }

    case 8:
    {
        printf("[ No. %3u ] Join Packet Received !!\n", id);

        Protocol::CS_Join Packet;
        Packet.ParseFromArray(buffer, bufferSize);

        printf("Player %d : join room %d", id, Packet.roomid());

        return true;
    }

    case 11:
    {
        printf("[ No. %3u ] Leave Packet Received !!\n", id);
        return true;
    }

    case 13:
    {
        printf("[ No. %3u ] Chatting Packet Received !!\n", id);

        Protocol::CS_Chatting Packet;
        Packet.ParseFromArray(buffer, bufferSize);

        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
        return true;
    }


    default: {
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
        return true;
    } break;
    }
}


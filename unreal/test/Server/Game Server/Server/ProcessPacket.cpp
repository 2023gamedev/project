#pragma once
#include"Common.h"

bool IOCP_CORE::IOCP_ProcessPacket(int id, Packet* buffer, int bufferSize) {
    // g_players에서 클라이언트 정보 검색
    auto it = g_players.find(id);
    if (it == g_players.end()) {
        // 유효하지 않은 클라이언트 ID에 대한 처리
        return false;
    }

    Protocol::Character tempPacket;
    tempPacket.ParseFromArray(buffer, bufferSize);

    // 패킷의 타입을 확인하여 처리
    switch (tempPacket.type()) {
    case 1: {
        // TestPacket을 처리하거나 이용
        //printf("[ No. %3u ] TEST Packet Received !!\n", id);
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
            // 자신에게는 패킷을 보내지 않음
            if (player.first != id) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    case 2: {

        Protocol::Zombie Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // 모든 연결된 클라이언트에게 패킷 전송 (브로드캐스팅)
        for (const auto& player : g_players) {
            // 자신에게는 패킷을 보내지 않음
            if (player.first != id) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    default: {
        // 알 수 없는 패킷 타입이 왔을 경우, 처리 방법을 결정하거나 오류 처리
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
        return false;
    } break;
    }
}


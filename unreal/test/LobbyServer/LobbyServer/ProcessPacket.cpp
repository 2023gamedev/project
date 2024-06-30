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

    Protocol::CS_Ready Packet;
    Packet.ParseFromArray(buffer, bufferSize);

    players_ready[Packet.playerid()] = Packet.ready();

    printf("receive ready packet\n");

    return true;

    //string serializedData;
    //Packet.SerializeToString(&serializedData);
    /*for (const auto& player : g_players) {
        if (player.first != id && player.second->isInGame) {
            IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
    }*/
}


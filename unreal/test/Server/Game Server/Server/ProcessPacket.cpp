#pragma once
#include"Common.h"

bool IOCP_CORE::IOCP_ProcessPacket(int id, Packet* buffer, int bufferSize) {
    // g_players���� Ŭ���̾�Ʈ ���� �˻�
    auto it = g_players.find(id);
    if (it == g_players.end()) {
        // ��ȿ���� ���� Ŭ���̾�Ʈ ID�� ���� ó��
        return false;
    }

    Protocol::Character tempPacket;
    tempPacket.ParseFromArray(buffer, bufferSize);

    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (tempPacket.type()) {
    case 1: {
        // TestPacket�� ó���ϰų� �̿�
        //printf("[ No. %3u ] TEST Packet Received !!\n", id);
        //printf("Received packet type = %d\n", CharacterPacket.type());
        //printf("Received playerID = %d\n", CharacterPacket.playerid());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
        //printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

        // ������ ���� ��Ŷ�� �״�� ������
        Protocol::Character Packet;
        Packet.ParseFromArray(buffer, bufferSize);
        string serializedData;
        Packet.SerializeToString(&serializedData);

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
        for (const auto& player : g_players) {
            // �ڽſ��Դ� ��Ŷ�� ������ ����
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

        // ��� ����� Ŭ���̾�Ʈ���� ��Ŷ ���� (��ε�ĳ����)
        for (const auto& player : g_players) {
            // �ڽſ��Դ� ��Ŷ�� ������ ����
            if (player.first != id) {
                IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
            }
        }
        return true;
    } break;

    default: {
        // �� �� ���� ��Ŷ Ÿ���� ���� ���, ó�� ����� �����ϰų� ���� ó��
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, tempPacket.type());
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
        return false;
    } break;
    }
}


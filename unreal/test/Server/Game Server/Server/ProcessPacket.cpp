#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, Protocol::Character CharacterPacket)
{
    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (CharacterPacket.type())
    {
    case 1:
    {
        // TestPacket�� ó���ϰų� �̿�
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", CharacterPacket.type());
        printf("Received playerID = %d\n", CharacterPacket.playerid());
        printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.x(), CharacterPacket.y(), CharacterPacket.z());
        printf("Received packet x = %f, y = %f, z = %f\n\n", CharacterPacket.pitch(), CharacterPacket.yaw(), CharacterPacket.roll());

        // ������ ���� ��Ŷ�� �״�� ������
        string serializedData;
        CharacterPacket.SerializeToString(&serializedData);

        if (id == 0) {
            IOCP_SendPacket(1, serializedData.data(), serializedData.size());
        }

        else IOCP_SendPacket(0, serializedData.data(), serializedData.size());
    }

    break;
    default:
    {
        // �� �� ���� ��Ŷ Ÿ���� ���� ���, ó�� ����� �����ϰų� ���� ó��
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, CharacterPacket.type());
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
    }
    break;
    }
}

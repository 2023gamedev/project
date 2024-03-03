#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, Protocol::TestPacket testPacket)
{
    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (testPacket.type())
    {
    case 1:
    {
        // TestPacket�� ó���ϰų� �̿�
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", testPacket.type());
        printf("Received playerID = %d\n", testPacket.playerid());
        printf("Received packet x = %f, y = %f, z = %f\n\n", testPacket.x(), testPacket.y(), testPacket.z());
        printf("Received packet x = %f, y = %f, z = %f\n\n", testPacket.pitch(), testPacket.yaw(), testPacket.roll());

        // ������ ���� ��Ŷ�� �״�� ������
        string serializedData;
        testPacket.SerializeToString(&serializedData);

        if (id == 0) {
            IOCP_SendPacket(1, serializedData.data(), serializedData.size());
        }

        else IOCP_SendPacket(0, serializedData.data(), serializedData.size());
    }

    break;
    default:
    {
        // �� �� ���� ��Ŷ Ÿ���� ���� ���, ó�� ����� �����ϰų� ���� ó��
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, testPacket.type());
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
    }
    break;
    }
}

#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, const Packet buf[])
{
    // ��Ŷ�� ũ�Ⱑ ������� Ȯ��
    if (buf[0] < sizeof(TestPacket))
    {
        printf("Received packet size is too small.\n");
        return;
    }

    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (buf[1])
    {
    case 1:
    {
        // ���� ��Ŷ�� TestPacket���� ��ȯ
        TestPacket receivedPacket;
        memcpy(&receivedPacket, &buf[0], sizeof(TestPacket));

        // TestPacket�� ó���ϰų� �̿�
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", receivedPacket.type);
        printf("Received packet x = %f, y = %f, z = %f\n\n", receivedPacket.x, receivedPacket.y, receivedPacket.z);

        // ������ ���� ��Ŷ�� �״�� ������
        IOCP_SendPacket(id, buf);
    }
    break;
    default:
    {
        // �� �� ���� ��Ŷ Ÿ���� ���� ���, ó�� ����� �����ϰų� ���� ó��
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, buf[1]);
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
    }
    break;
    }
}

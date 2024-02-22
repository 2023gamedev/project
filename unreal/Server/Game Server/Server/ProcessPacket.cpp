#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, const char buf[], size_t bufSize)
{
    Protocol::TestPacket receivedPacket;

    // ���ۿ��� Protobuf �޽��� ������ȭ
    if (!receivedPacket.ParseFromArray(buf, bufSize))
    {
        printf("Failed to parse received packet.\n");
        return;
    }

    // ��Ŷ�� Ÿ���� Ȯ���Ͽ� ó��
    switch (receivedPacket.type())
    {
    case 1:
    {
        // TestPacket�� ó���ϰų� �̿�
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", receivedPacket.type());
        printf("Received packet x = %f, y = %f, z = %f\n\n", receivedPacket.x(), receivedPacket.y(), receivedPacket.z());

        // ������ ���� ��Ŷ�� �״�� ������
        string serializedData;
        receivedPacket.SerializeToString(&serializedData);
        IOCP_SendPacket(id, serializedData.data(), serializedData.size());
    }
    break;
    default:
    {
        // �� �� ���� ��Ŷ Ÿ���� ���� ���, ó�� ����� �����ϰų� ���� ó��
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, receivedPacket.type());
        // Ŭ���̾�Ʈ�� ���� ����, �α� ���� ó�� ����
    }
    break;
    }
}

#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, const char buf[], size_t bufSize)
{
    Protocol::TestPacket receivedPacket;

    // 버퍼에서 Protobuf 메시지 역직렬화
    if (!receivedPacket.ParseFromArray(buf, bufSize))
    {
        printf("Failed to parse received packet.\n");
        return;
    }

    // 패킷의 타입을 확인하여 처리
    switch (receivedPacket.type())
    {
    case 1:
    {
        // TestPacket을 처리하거나 이용
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", receivedPacket.type());
        printf("Received packet x = %f, y = %f, z = %f\n\n", receivedPacket.x(), receivedPacket.y(), receivedPacket.z());

        // 서버로 받은 패킷을 그대로 돌려줌
        string serializedData;
        receivedPacket.SerializeToString(&serializedData);
        IOCP_SendPacket(id, serializedData.data(), serializedData.size());
    }
    break;
    default:
    {
        // 알 수 없는 패킷 타입이 왔을 경우, 처리 방법을 결정하거나 오류 처리
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, receivedPacket.type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
    }
    break;
    }
}

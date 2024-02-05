#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, const Packet buf[])
{
    // 패킷의 크기가 충분한지 확인
    if (buf[0] < sizeof(TestPacket))
    {
        printf("Received packet size is too small.\n");
        return;
    }

    // 패킷의 타입을 확인하여 처리
    switch (buf[1])
    {
    case 1:
    {
        // 받은 패킷을 TestPacket으로 변환
        TestPacket receivedPacket;
        memcpy(&receivedPacket, &buf[0], sizeof(TestPacket));

        // TestPacket을 처리하거나 이용
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", receivedPacket.type);
        printf("Received packet x = %f, y = %f, z = %f\n\n", receivedPacket.x, receivedPacket.y, receivedPacket.z);

        // 서버로 받은 패킷을 그대로 돌려줌
        IOCP_SendPacket(id, buf);
    }
    break;
    default:
    {
        // 알 수 없는 패킷 타입이 왔을 경우, 처리 방법을 결정하거나 오류 처리
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, buf[1]);
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
    }
    break;
    }
}

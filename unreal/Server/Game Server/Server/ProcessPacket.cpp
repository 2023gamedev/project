#pragma once
#include"Common.h"

void IOCP_CORE::IOCP_ProcessPacket(const unsigned int& id, Protocol::TestPacket testPacket)
{
    // 패킷의 타입을 확인하여 처리
    switch (testPacket.type())
    {
    case 1:
    {
        // TestPacket을 처리하거나 이용
        printf("[ No. %3u ] TEST Packet Received !!\n", id);
        printf("Received packet type = %d\n", testPacket.type());
        printf("Received playerID = %d\n", testPacket.playerid());
        printf("Received packet x = %f, y = %f, z = %f\n\n", testPacket.x(), testPacket.y(), testPacket.z());
        printf("Received packet x = %f, y = %f, z = %f\n\n", testPacket.pitch(), testPacket.yaw(), testPacket.roll());

        // 서버로 받은 패킷을 그대로 돌려줌
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
        // 알 수 없는 패킷 타입이 왔을 경우, 처리 방법을 결정하거나 오류 처리
        printf("ERROR, Unknown signal -> [ %u ] protocol num = %d\n", id, testPacket.type());
        // 클라이언트나 서버 종료, 로깅 등의 처리 가능
    }
    break;
    }
}

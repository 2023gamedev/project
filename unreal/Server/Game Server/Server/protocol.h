#pragma once

#define SERVERPORT 7777
#define MAX_BUF_SIZE 256
#define MAX_USER 500

// packet[1] operation
#define DISCONNECTED 0

// iocp buf operation
#define OP_SERVER_RECV 1
#define OP_SERVER_SEND 2

typedef unsigned char uint8;

// process protocol
enum PacketProtocolType {
	TEST = 1
};

struct TestPacket {
    uint8 packet_size;
    uint8 type;
    float x;
    float y;
    float z;
};

using Packet = unsigned char;
#pragma once

#define SERVERPORT 8777
#define MAX_BUF_SIZE 8192
#define MAX_USER 500
#define MAX_PACKET_SIZE 8192

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

using Packet = unsigned char;
#include "DummyClient.h"
#include "winsock2.h"

DummyClient::DummyClient()
{

}

DummyClient::~DummyClient()
{

}

void DummyClient::Init()
{

	// init Winsock
	if (WSAStartup(MAKEWORD(2, 2), &m_wsadata) != 0) {
		err_quit("WSAStartup ERROR");
	}

	m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	if (INVALID_SOCKET == m_sock) {
		err_quit("socket()");
	}

	// connect
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(7777);

	m_retval = WSAConnect(m_sock, (SOCKADDR*)&serveraddr, 
						  sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (SOCKET_ERROR == m_retval) {
		// 비동기 소켓이라 그냥 리턴, 검사 해주어야 함
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			err_quit("connect()");
		}
	}

}

void DummyClient::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
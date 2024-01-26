#pragma once
class DummyClient
{

public:
	DummyClient();
	~DummyClient();

	void Init();

private:

	void err_quit(const char* msg);

	WSADATA m_wsadata;
	SOCKET m_sock;
	int m_retval{ 0 };
};


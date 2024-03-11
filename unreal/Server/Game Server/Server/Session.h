#pragma once

#include "Common.h"

class Session
{
public:
	Session();
	~Session();

	static Session* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new Session;	//�ν��Ͻ��� ���ٸ� �ϳ� �����մϴ�.

		return m_pInstance;
	}

	unsigned int getPlayerIndex() const;

private:
	static Session* m_pInstance;

private:
	static unsigned int nextPlayerIndex;
	unsigned int playerIndex;

};


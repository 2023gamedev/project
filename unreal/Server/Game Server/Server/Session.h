#pragma once

class Session
{
public:
	Session();
	~Session();

	unsigned int getPlayerIndex() const;

private:
	static unsigned int nextPlayerIndex;
	unsigned int playerIndex;

};


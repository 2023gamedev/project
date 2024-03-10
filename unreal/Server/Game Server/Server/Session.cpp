#include "Session.h"

Session::Session()
{
	playerIndex = nextPlayerIndex++;
}

Session::~Session()
{
}

unsigned int Session::nextPlayerIndex = 1;

unsigned int Session::getPlayerIndex() const
{
	return playerIndex;
}


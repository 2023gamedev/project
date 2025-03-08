#pragma once

#include "Zombie.h"


class ShoutingZombie : public Zombie
{
public:
	bool bShouted;


	ShoutingZombie();

	ShoutingZombie(IOCP_CORE* mainServer, Zombie_Data zd);

	void Shout(int playerid, int roomid);

};
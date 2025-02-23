#pragma once

#include "Zombie.h"


class ShoutingZombie : public Zombie
{
public:
	bool bShouted;


	ShoutingZombie();

	ShoutingZombie(IOCP_CORE* mainServer, Zombie_Data zd);

	void Shout(vector<Zombie*>& zombies, int playerid, int roomid);

};
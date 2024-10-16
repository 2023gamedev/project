#pragma once

#include "Zombie.h"

class ShoutingZombie : public Zombie
{
public:
	ShoutingZombie();

	ShoutingZombie(Zombie_Data zd);

	Shout();
};
#pragma once

#include "Zombie.h"

class ShoutingZombie : public Zombie
{
public:
	bool bShouted;

	bool IsShouting;	// 해당 좀비 지금 샤우팅 중인가? (애니메이션 재생 중인 가?)

	ShoutingZombie();

	ShoutingZombie(Zombie_Data zd);

	void Shout(vector<Zombie*>& zombies);
};
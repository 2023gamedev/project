#include "NormalZombie.h"

#include "ZombiePathfinder.h"


class ZombiePathfinder;


NormalZombie::NormalZombie() 
	: Zombie()
{

}

NormalZombie::NormalZombie(IOCP_CORE* mainServer, Zombie_Data z_d)
	: Zombie(mainServer, z_d)
{
	ZombieStartHP = NormalZombieStartHP;
	SetHP(ZombieStartHP);

	//SetSpeed(NormalZombieSpeed);

	SetZombieType(ZOMBIE_TYPE::NORMAL_ZOMBIE);

	runawayHealthRegenInterval = 2;
	runawayHealthRegenPoint = 1;

}
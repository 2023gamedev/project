#include "RunningZombie.h"

#include "ZombiePathfinder.h"


class ZombiePathfinder;


RunningZombie::RunningZombie()
	: Zombie()
{
	
}

RunningZombie::RunningZombie(IOCP_CORE* mainServer, Zombie_Data z_d)
	: Zombie(mainServer, z_d)
{
	ZombieStartHP = RunningZombieStartHP;
	SetHP(ZombieStartHP);
	
	//SetSpeed(RunningZombieSpeed);

	SetZombieType(ZOMBIE_TYPE::RUNNING_ZOMBIE);

}
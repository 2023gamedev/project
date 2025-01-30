#include "RunningZombie.h"

#include "ZombiePathfinder.h"

class ZombiePathfinder;

RunningZombie::RunningZombie()
	: Zombie()
{
	
}

RunningZombie::RunningZombie(Zombie_Data z_d)
	: Zombie(z_d)
{
	SetHP(RunningZombieStartHP);
	
	//SetSpeed(RunningZombieSpeed);

	SetZombieType(ZOMBIE_TYPE::RUNNING_ZOMBIE);

}
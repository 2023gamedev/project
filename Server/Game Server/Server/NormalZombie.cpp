#include "NormalZombie.h"

#include "ZombiePathfinder.h"

class ZombiePathfinder;

NormalZombie::NormalZombie() 
	: Zombie()
{

}

NormalZombie::NormalZombie(Zombie_Data z_d)
	: Zombie(z_d)
{
	SetHP(NormalZombieStartHP);

	SetSpeed(NormalZombieSpeed);

}
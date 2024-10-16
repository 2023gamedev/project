#include "ShoutingZombie.h"

#include "ZombiePathfinder.h"
#include "iocpServerClass.h"


ShoutingZombie::ShoutingZombie()
	: Zombie()
{
	SetHP(0);

	SetSpeed(0);
}

ShoutingZombie::ShoutingZombie(Zombie_Data z_d)
	: Zombie(z_d)
{
	SetHP(ShoutingZombieStartHP);

	SetSpeed(ShoutingZombieSpeed);
}

void Shout()
{
	
}
#include "Zombie.h"

float Zombie::DistanceToPlayer = 1000.f;

const float Zombie::CanSeeDistance = 50.f;

const float Zombie::CanAttackDistance = 10.f;


void Zombie::SetDistance(float dtp)
{ 
	Zombie::DistanceToPlayer = dtp; 
}
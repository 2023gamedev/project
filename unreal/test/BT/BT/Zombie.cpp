#include "Zombie.h"


float Zombie::DistanceToPlayer = 1000.f;

const float Zombie::CanSeeDistance = 50.f;

const float Zombie::CanAttackDistance = 10.f;

vector<vector<vector<int>>> Zombie::PlayerLastLocation = vector<vector<vector<int>>>{};

bool Investigated = false;

bool HeardFootSound = false;

bool HeardShouting = false;


void Zombie::SetDistance(float dtp)
{ 
	Zombie::DistanceToPlayer = dtp; 
}

void Zombie::SetPlayerLocation(vector<vector<vector<int>>> pll)
{
	Zombie::PlayerLastLocation = pll;
}
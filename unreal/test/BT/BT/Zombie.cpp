#include <iostream>

#include "Zombie.h"

using namespace std;


Zombie::Zombie()
{
	name = string{ "" };

	DistanceToPlayer = 1000.f;

	TargetLocation = vector<vector<vector<int>>>{ {{0, 0, 0}} };

	KnewPlayerLocation = false;

	HeardFootSound = false;

	HeardShouting = false;
}

Zombie::Zombie(string n, float dtp, vector<vector<vector<int>>> tl, bool kpl, bool hfs, bool hs)
{
	name = n;

	DistanceToPlayer = dtp;

	TargetLocation = tl;

	KnewPlayerLocation = kpl;

	HeardFootSound = hfs;

	HeardShouting = hs;
}


void Zombie::SetDistance(float dtp)
{ 
	Zombie::DistanceToPlayer = dtp; 
}

void Zombie::SetPlayerLocation(vector<vector<vector<int>>> pll)
{
	Zombie::TargetLocation = pll;
}

void Zombie::Attack() const
{
	cout << "Zombie " << name << " attacked player!" << endl;
}

void Zombie::MoveTo() const
{
	cout << "Zombie " << name << " moved to (" << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << ")." << endl;
}

void Zombie::Wait() const
{
	cout << "Zombie " << name << " wait." << endl;
}
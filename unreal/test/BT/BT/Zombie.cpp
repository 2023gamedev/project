#include <iostream>
#include <cmath>

#include "Zombie.h"

using namespace std;


Zombie::Zombie()
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = new Player;

	name = string{ "" };

	ZombieLocation = vector<vector<vector<int>>>{ {{0, 0, 0}} };

	ZombieOriginLocation = ZombieLocation;

	DistanceToPlayer = 1000.f;

	TargetLocation = vector<vector<vector<int>>>{ {{0, 0, 0}} };

	KnewPlayerLocation = false;

	HeardFootSound = false;

	HeardShouting = false;
}

Zombie::Zombie(Player* p, string n, vector<vector<vector<int>>> zl)
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = p;

	name = n;

	ZombieLocation = zl;

	ZombieOriginLocation = ZombieLocation;

	vector<vector<vector<int>>> pl = p->GetPlayerPos();

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	TargetLocation = vector<vector<vector<int>>>{ {{0, 0, 0}} };

	KnewPlayerLocation = false;

	HeardFootSound = false;

	HeardShouting = false;
}


void Zombie::SetDistance()
{ 
	vector<vector<vector<int>>> zl = ZombieLocation;
	vector<vector<vector<int>>> pl = PL->GetPlayerPos();

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));
}

void Zombie::SetTargetLocation(TARGET t)
{
	switch (t) {
	case TARGET::PLAYER:
		TargetLocation = PL->GetPlayerPos();
		break;
	case TARGET::SHOUTING:
		//샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요
		break;
	case TARGET::FOOTSOUND:
		TargetLocation = PL->GetPlayerPos();
		break;
	case TARGET::INVESTIGATED:
		break;
	case TARGET::ORIGIN:
		TargetLocation = ZombieOriginLocation;
		break;
	}
}

void Zombie::Attack() const
{
	cout << "Zombie \'" << name << "\' attacked Player!" << endl;
}

void Zombie::MoveTo() const
{
	cout << "Zombie \'" << name << "\' moved to ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )." << endl;
}

void Zombie::Wait() const
{
	cout << "Zombie \'" << name << "\' wait." << endl;
}
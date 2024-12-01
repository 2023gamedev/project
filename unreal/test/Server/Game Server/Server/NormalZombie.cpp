#include "NormalZombie.h"

#include "ZombiePathfinder.h"

class ZombiePathfinder;

NormalZombie::NormalZombie() 
	: Zombie()
{
	/*	// 일일히 할당할 필요 없음 - 생성자 초기화 리스트 " : Zombie() " 를 이미 했으니
	SetHP(0);

	SetSpeed(0);

	path = vector<tuple<float, float, float>>{};

	beforepath = vector<tuple<float, float, float>>{};

	ZombieData = Zombie_Data();

	DistanceTo_PlayerInsight = {};

	DistanceTo_FootSound = {};

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PrevTargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	RandPatrolSet = false;

	IsAttacking = false;

	IsBeingAttacked = false;

	IsShouting = false;

	HaveToWait = false;

	DetermineFloor(ZombieData.z);

	//speed = 0.f;

	targetType = Zombie::TARGET::NULL_TARGET;

	ClosestPlayerID = 0;
	*/
}

NormalZombie::NormalZombie(Zombie_Data z_d)
	: Zombie(z_d)
{
	
	SetHP(NormalZombieStartHP);

	SetSpeed(NormalZombieSpeed);

	/*
	path = vector<tuple<float, float, float>>{}; 
	
	beforepath = vector<tuple<float, float, float>>{};

	ZombieData = z_d;

	DistanceTo_PlayerInsight = {};

	DistanceTo_FootSound = {};

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PrevTargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	RandPatrolSet = false;

	IsAttacking = false;

	IsBeingAttacked = false;

	IsShouting = false;

	HaveToWait = false;

	DetermineFloor(ZombieData.z);

	//speed = 0.f;

	targetType = Zombie::TARGET::PATROL;

	ClosestPlayerID = 0;
	*/
}
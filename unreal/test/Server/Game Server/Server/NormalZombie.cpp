#include "NormalZombie.h"

#include "ZombiePathfinder.h"

class ZombiePathfinder;

NormalZombie::NormalZombie()
	//: pathfinder(0, 0, 0, 1, 1, 1) // 사용X -> 초기화 리스트는 부모 클래스의 생성자를 먼저 부르므로, 대신 이렇게
	: Zombie()		// 근데 사실 의미 없는게, 이렇게 초기화 리스트 안해도 부모 클래스에서 알아서 자동으로 기본 생성자 불러서 초기화 시킴
{

	cout << "NormalZombie()" << endl;
	SetHP(-2);
	cout << GetHP() << endl;

	path = vector<tuple<float, float, float>>{};

	beforepath = vector<tuple<float, float, float>>{};

	ZombieData = Zombie_Data();

	DistanceToPlayers = {};

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PrevTargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	RandPatrolSet = false;

	IsAttacking = false;

	//speed = 0.f;

	targetType = Zombie::TARGET::NULL_TARGET;

	bt_playerID = 0;
}

NormalZombie::NormalZombie(Zombie_Data z_d)
	//: pathfinder(0, 0, 0, 1, 1, 1)
	: Zombie(z_d)
{
	cout << "NormalZombie(Zombie_Data z_d)" << endl;
	SetHP(20);
	cout << GetHP() << endl;

	path = vector<tuple<float, float, float>>{}; 

	ZombieData = z_d;

	DistanceToPlayers = {};

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PrevTargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	RandPatrolSet = false;

	IsAttacking = false;

	//speed = 0.f;

	targetType = Zombie::TARGET::PATROL;

	bt_playerID = 0;
}
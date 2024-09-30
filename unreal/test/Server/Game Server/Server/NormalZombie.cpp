#include "NormalZombie.h"

#include "ZombiePathfinder.h"

class ZombiePathfinder;

NormalZombie::NormalZombie()
	//: pathfinder(0, 0, 0, 1, 1, 1) // ���X -> �ʱ�ȭ ����Ʈ�� �θ� Ŭ������ �����ڸ� ���� �θ��Ƿ�, ��� �̷���
	: Zombie()		// �ٵ� ��� �ǹ� ���°�, �̷��� �ʱ�ȭ ����Ʈ ���ص� �θ� Ŭ�������� �˾Ƽ� �ڵ����� �⺻ ������ �ҷ��� �ʱ�ȭ ��Ŵ
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
#include <iostream>
#include <cmath>

#include "Zombie.h"
#include "ZombiePathfinder.h"   // PathFinder
#include"iocpServerClass.h"		// 전역변수 playerDB 사용하려구

using std::cout;
using std::endl;


Zombie::Zombie()
{
	path = vector<tuple<float, float, float>>{};

	ZombieData = Zombie_Data();

	ZombieOriginLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	DistanceToPlayer = 100000.f;		//그냥 초기화값

	TargetLocation = ZombieOriginLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 0.f;

	targetType = Zombie::TARGET::ORIGIN;

	bt_playerID = 0;
}

Zombie::Zombie(Zombie_Data z_d, vector<vector<vector<float>>> zl)
{
	path = vector<tuple<float, float, float>>{};

	ZombieData = z_d;

	ZombieOriginLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	DistanceToPlayer = 100000.f;		//그냥 초기화값

	TargetLocation = ZombieOriginLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 1.f;

	targetType = Zombie::TARGET::ORIGIN;

	bt_playerID = 0;
}

Zombie::~Zombie()
{
	//여기 cout 해서 출력보면 왜 zombie 클래스가 생성도 되기 전인데 여러번 미리 불림?! 
	// -> 그래서 여기서 만약 new 할당된 메모리 delete 하려하면 할당된 메모리도 없는데 지울려 해서 에러 (지금은 다 없애어 없긴하지만... 미리 소멸자가 여러번 불리는 건 마찬가지)
	// -> 이유는 모르겠음 그냥
}


void Zombie::SetDistance()
{ 
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB[bt_playerID].x, playerDB[bt_playerID].y, playerDB[bt_playerID].z}}};

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));
}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB[bt_playerID].x, playerDB[bt_playerID].y, playerDB[bt_playerID].z}} };

	switch (targetType) {
	case TARGET::PLAYER:
		TargetLocation = pl;
		break;
	case TARGET::SHOUTING:
		//==================================샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요
		break;
	case TARGET::FOOTSOUND:
		TargetLocation = pl;
		break;
	case TARGET::INVESTIGATED: 
		TargetLocation = TargetLocation;		//걍 명시적 표기
		break;
	case TARGET::ORIGIN:
		TargetLocation = ZombieOriginLocation;	//============================원래 자리로 돌아가기 보다는 랜덤한 근처 장소로 이동하게 만들어서 배회하게끔 만들면 좋을 듯
		break;
	}
}

void Zombie::Attack()
{
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어 \'#" << bt_playerID << "\' 을 공격하였습니다!" << endl;
	cout << endl;
}

void Zombie::Walk()
{

	float PathX = get<0>(path.front());
	float PathY = get<1>(path.front());
	if (ZombieData.x == PathX && ZombieData.y == PathY) {
		PathX = get<0>(path[1]);
		PathY = get<1>(path[1]);
	}


	cout << "좀비 \'#" << ZombieData.zombieID << "\' WALK ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;
}


void Zombie::MoveTo()
{
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> tl = TargetLocation;

	//===================================
	ZombiePathfinder pathfinder(zl[0][0][0], zl[0][0][1], zl[0][0][2], tl[0][0][0], tl[0][0][1], tl[0][0][2]);
	pathfinder.Run(path);

	cout << endl;
	if(path.size() != 0)
		cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 경로의 첫 좌표: ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;
	
	//===================================

	Walk();
	// 장애물에서 장애물 근처 초록색 좌표 추가해서 B1 이런데 추가하기

	//===================================

	// PathFinder로부터 경로 리스트 쭉 받고, 이를 가지고 언리얼에서 돌아가는 MoveTo와 유사하게 직접 여기서 움직임 계산하고 위치를 이동시켜서, 바뀐 좌표를 클라에게 쏘게 하는 방식 *(회의 필요) 

	cout << endl;
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )." << endl;
	cout << endl;


	//좀비가 목적지에 도착하면
	zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	if (zl == TargetLocation) {

		cout << "좀비 \#'" << ZombieData.zombieID << "\' 타겟 좌표 ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " ) 에 도착!!!" << endl;
		cout << endl;

		//<Selector Detect>의 Task들의 실행 조건이 되는 bool값들 초기화
		switch (targetType) {
		case TARGET::PLAYER:
			//사실상 실행될 일 없음
			break;
		case TARGET::SHOUTING:
			HeardShouting = false;
			break;
		case TARGET::FOOTSOUND:
			HeardFootSound = false;
			break;
		case TARGET::INVESTIGATED:
			KnewPlayerLocation = false;
			break;
		case TARGET::ORIGIN:
			//원래 자리로 돌아가기 보다는 랜덤한 근처 장소로 이동하게 만들어서 배회하게끔 만들면 좋을 듯 => 배회 중 목적지 닿으면 또 근처 장소 랜덤하게 타겟 잡아서 다시 이동
			break;
		}

	}

	//cout << endl;
}

void Zombie::Wait()
{
	//cout << "Zombie \'#" << ZombieData.zombieID << "\' wait for delay." << endl;
	//cout << endl;
}
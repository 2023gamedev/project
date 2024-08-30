#include <iostream>
#include <cmath>

#include "Zombie.h"
#include "ZombiePathfinder.h"   // PathFinder

using namespace std;


Zombie::Zombie()
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = new Player;

	path = vector<tuple<float, float, float>>{};

	//name = string{ "" };
	ZombieData.zombieID = 0;

	//ZombieLocation = vector<vector<vector<float>>>{ {{0, 0, 0}} };
	ZombieData.x = 0;	ZombieData.y = 0;	ZombieData.z = 0; 

	ZombieOriginLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	DistanceToPlayer = 1000.f;

	TargetLocation = ZombieOriginLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 0;

	targetType = Zombie::TARGET::ORIGIN;
}

Zombie::Zombie(int z_ID, Player* p, string n, vector<vector<vector<float>>> zl)
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = p;

	path = vector<tuple<float, float, float>>{};

	//name = n;
	ZombieData.zombieID = z_ID;

	//ZombieLocation = zl;
	ZombieData.x = zl[0][0][0];	ZombieData.y = zl[0][0][1];	ZombieData.z = zl[0][0][2];

	ZombieOriginLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	vector<vector<vector<float>>> pl = p->PlayerLocation;

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	TargetLocation = ZombieOriginLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 1;

	targetType = Zombie::TARGET::ORIGIN;
}


void Zombie::SetDistance()
{ 
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = PL->PlayerLocation;

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));
}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

	switch (t) {
	case TARGET::PLAYER:
		TargetLocation = PL->PlayerLocation;
		break;
	case TARGET::SHOUTING:
		//샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요
		break;
	case TARGET::FOOTSOUND:
		TargetLocation = PL->PlayerLocation;
		break;
	case TARGET::INVESTIGATED: 
		TargetLocation = TargetLocation;		//걍 명시적 표기
		break;
	case TARGET::ORIGIN:
		TargetLocation = ZombieOriginLocation;	//원래 자리로 돌아가기 보다는 랜덤한 근처 장소로 이동하게 만들어서 배회하게끔 만들면 좋을 듯
		break;
	}
}

void Zombie::Attack()
{
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어를 공격하였습니다!" << endl;
	cout << endl;
}

void Zombie::MoveTo()
{
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> tl = TargetLocation;

	//===================================
	ZombiePathfinder pathfinder(zl[0][0][0], zl[0][0][1], zl[0][0][2], tl[0][0][0], tl[0][0][1], tl[0][0][2]);
	pathfinder.Run(path);

	cout << endl;
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 경로의 첫 좌표: ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;

	//===================================

	// 장애물에서 장애물 근처 초록색 좌표 추가해서 B1 이런데 추가하기

	//===================================

	// PathFinder로부터 경로 리스트 쭉 받고, 이를 가지고 언리얼에서 돌아가는 MoveTo와 유사하게 직접 여기서 움직임 계산하고 위치를 이동시켜서, 바뀐 좌표를 클라에게 쏘게 하는 방식 *(회의 필요) 
	// Can See Player 조건식 지우고 클라이언트에서 검사한 값을 서버로 보내주고 이를 받아서(zom.KnewPlayerLocation) 조건 검사하는 조건식 짜기

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
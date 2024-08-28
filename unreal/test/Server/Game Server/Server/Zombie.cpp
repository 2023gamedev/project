#include <iostream>
#include <cmath>

#include "Zombie.h"

// PathFinder
#include "ZombiePathfinder.h"

using namespace std;


Zombie::Zombie()
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = new Player;

	name = string{ "" };

	ZombieLocation = vector<vector<vector<float>>>{ {{0, 0, 0}} };

	ZombieOriginLocation = ZombieLocation;

	DistanceToPlayer = 1000.f;

	TargetLocation = ZombieLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 0;

	targetType = Zombie::TARGET::ORIGIN;
}

Zombie::Zombie(Player* p, string n, vector<vector<vector<float>>> zl)
{
	Z_BT = new vector<unique_ptr<Task>>;

	PL = p;

	name = n;

	ZombieLocation = zl;

	ZombieOriginLocation = ZombieLocation;

	vector<vector<vector<float>>> pl = p->GetPlayerPos();

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	TargetLocation = ZombieLocation;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	speed = 1;

	targetType = Zombie::TARGET::ORIGIN;
}


void Zombie::SetDistance()
{ 
	vector<vector<vector<float>>> zl = ZombieLocation;
	vector<vector<vector<float>>> pl = PL->GetPlayerPos();

	DistanceToPlayer = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));
}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

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
		TargetLocation = TargetLocation;		//걍 명시적 표기
		break;
	case TARGET::ORIGIN:
		TargetLocation = ZombieOriginLocation;	//원래 자리로 돌아가기 보다는 랜덤한 근처 장소로 이동하게 만들어서 배회하게끔 만들면 좋을 듯
		break;
	}
}

void Zombie::Attack()
{
	cout << "Zombie \'" << name << "\' attacks Player!" << endl;
	cout << endl;
}

void Zombie::MoveTo()
{
	cout << "Zombie \'" << name << "\' moves to Target ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )." << endl;

	vector<vector<vector<float>>> zl = ZombieLocation;
	vector<vector<vector<float>>> tl = TargetLocation;

	//===================================
	ZombiePathfinder pathfinder(zl[0][0][0], zl[0][0][1], zl[0][0][2], tl[0][0][0], tl[0][0][1], tl[0][0][2]);
	pathfinder.Run();

	// PathFinder에서 이동하는 처음 좌표 하나 보내주기(리턴값)
	// 장애물에서 장애물 근처 초록색 좌표 추가해서 B1 이런데 추가하기

	//===================================

	// Move To 좌표 값들 float 값으로 바꿔주기
	// Can See Player 조건값 지우고 클라이언트에서 검사한 값을 서버로 보내주게 조건식 짜기


	// 공통
	// 필요없는 Cout 지우기

	cout << "좀비 " << name << " ";
	if (zl[0][0][0] < tl[0][0][0]) {
		ZombieLocation[0][0][0] += speed;
		cout << "x축으로 " << speed << ", ";
	}
	else if (zl[0][0][0] > tl[0][0][0]) {
		ZombieLocation[0][0][0] -= speed;
		cout << "x축으로 " << -speed << " , ";
	}
	else {
		cout << "x축으로 " << 0 << " , ";
	}

	if (zl[0][0][1] < tl[0][0][1]) {
		ZombieLocation[0][0][1] += speed;
		cout << "y축으로 " << speed << " ";
	}
	else if (zl[0][0][1] > tl[0][0][1]) {
		ZombieLocation[0][0][1] -= speed;
		cout << "y축으로 " << -speed << " ";
	}
	else {
		cout << "y축으로 " << 0 << " ";
	}
	cout << "이동!!!" << endl;


	//좀비가 목적지에 도착하면
	if (ZombieLocation == TargetLocation) {

		cout << "Zombie \'" << name << "\' arrived Target ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )." << endl;

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

	cout << endl;
}

void Zombie::Wait()
{
	//cout << "Zombie \'" << name << "\' wait for delay." << endl;
	//cout << endl;
}
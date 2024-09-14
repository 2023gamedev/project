#include <iostream>
#include <cmath>
#include <random>

#include "GStruct.pb.h"
#include "iocpServerClass.h"

#include "Zombie.h"
#include"iocpServerClass.h"		// 전역변수 playerDB 사용하려구

using std::cout;
using std::endl;


Zombie::Zombie()
	: pathfinder(0.f, 0.f, 0.f, 1.f, 1.f, 1.f)
{
	path = vector<tuple<float, float, float>>{};

	beforepath = vector<tuple<float, float, float>>{};

	ZombieData = Zombie_Data();

	DistanceToPlayer = 100000.f;		//그냥 초기화값

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	SetRandPatrol = false;

	speed = 0.f;

	targetType = Zombie::TARGET::PATROL;


	bt_playerID = 0;
}

Zombie::Zombie(Zombie_Data z_d, vector<vector<vector<float>>> zl)
{
	path = vector<tuple<float, float, float>>{};

	ZombieData = z_d;

	DistanceToPlayer = 100000.f;		//그냥 초기화값

	TargetLocation = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };

	PlayerInSight = false;

	KnewPlayerLocation = false;

	HeardShouting = false;

	HeardFootSound = false;

	SetRandPatrol = false;

	speed = 1.f;

	targetType = Zombie::TARGET::PATROL;

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

bool Zombie::RandomPatrol()
{

	cout << "RandomPatrol!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

	float px, py, pz;

	std::random_device rd;
	std::mt19937 mt(rd());
	
	std::uniform_int_distribution<int> dist(-500, 500);		//현 위치에서 반경 500 +-

	cout << "RandomPatrol2222!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

	px = ZombieData.x + dist(mt);
	py = ZombieData.y + dist(mt);
	pz = ZombieData.z;

	vector<tuple<float, float, float>> tmp;
	ZombiePathfinder pathfinderpatrol(ZombieData.x, ZombieData.y, ZombieData.z, px, py, pz);
	pathfinderpatrol.Run(tmp, 1);

	if (!tmp.empty()) {
		tmp.pop_back();
		
		tuple<float, float, float> t_tmp;
		t_tmp = tmp.back();

		TargetLocation[0][0][0] = get<0>(t_tmp);
		TargetLocation[0][0][1] = get<1>(t_tmp);
		TargetLocation[0][0][2] = pz;

		//tmp.push_back(t_tmp);

		pathfinder.UpdatePathFinder(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
		path = tmp;

		SetRandPatrol = true;

		return true;
	}
	else
		return false;

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
	case TARGET::PATROL:
		//============================랜덤한 근처 장소로 이동하게 만들어서 배회
		int cnt = 0;
		if (SetRandPatrol == false)
			while (RandomPatrol() == false) {
				cnt++;

				if (cnt >= 5)
					break;
			}
		break;
	}
}

void Zombie::Attack()
{
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어 \'#" << bt_playerID << "\' 을 공격하였습니다!" << endl;
	cout << endl;
}


void Zombie::Walk(float deltasecond)
{

	if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1]) {
		//cout << "return" << endl;
		return;
	}

	if (IsPathUpdated()) {
		cout << "PathUpdate" << endl;
		ZombiePathIndex = 0;
	}

	//Walk가 동작하기 전에 이미 도착위치에 위치해 있다면, 해당 종료조건 발동 X
	if (ZombiePathIndex >= path.size()) {
		//cout << "Zombie has reached the final destination." << endl;

		return; // 경로 끝에 도달
	}



	// 현재 목표 노드
	tuple<float, float, float> TargetNode = path[ZombiePathIndex];
	float PathX = get<0>(TargetNode);
	float PathY = get<1>(TargetNode);

	float ZombieSpeed = 0.f;
	if (ZombieData.zombietype == 0) {
		ZombieSpeed = 200.f;
	}
	else if (ZombieData.zombietype == 1) {
		ZombieSpeed = 300.f;
	}
	else if (ZombieData.zombietype == 2) {
		ZombieSpeed = 400.f;
	}
	else {
		cout << "WALK ERROR" << endl;
		return;
	}



	// 타겟 방향 계산
	float dx = PathX - ZombieData.x;
	float dy = PathY - ZombieData.y;

	// 거리를 계산
	float distance = sqrt(dx * dx + dy * dy);

	// 이동 방향 벡터를 정규화
	float directionX = dx / distance;
	float directionY = dy / distance;

	// 이동 거리 계산
	float moveDistance = ZombieSpeed * deltasecond;

	// 이동 벡터 계산
	float moveX = directionX * moveDistance;
	float moveY = directionY * moveDistance;


	// 타겟 위치에 도달했는지 확인
	// 위에 종료 조건인 if (ZombiePathIndex >= path.size()) 이 종료시점을 놓쳐도 해당 조건식이 발동되어서 도착지점에서 멈춰야 하는데 그렇지 않은 것 같음

	// 목표에 도착했는지 확인 (옵션)
	float newDistance = sqrt((PathX - ZombieData.x) * (PathX - ZombieData.x) + (PathY - ZombieData.y) * (PathY - ZombieData.y));

	if (newDistance < moveDistance) {
		ZombieData.x = PathX;
		ZombieData.y = PathY;

		//cout << "speed * deltasecond " << max(ZombieSpeed * deltasecond, 1e-5f) << endl;
		//cout << "distance : " << distance << endl;
		//cout << "X: " << ZombieData.x << endl;
		//cout << "Y: " << ZombieData.y << endl;

		// 다음 목표 노드로 이동
		ZombiePathIndex++;

		if (ZombiePathIndex >= path.size()) {
			cout << "Zombie 경로 끝." << endl;
		}
	}
	else {
		// 타겟 방향으로 이동
		ZombieData.x += moveX;
		ZombieData.y += moveY;
		//cout << "X: " << ZombieData.x << endl;
		//cout << "Y: " << ZombieData.y << endl;
	}
}

bool Zombie::IsPathUpdated()
{


	if (!path.empty() && !beforepath.empty()) {

		if (beforepath == path) {
			return false;
		}
		else {

			beforepath = path;
			return true;
		}
		
	}

	if (!path.empty() && beforepath.empty()) {

		beforepath = path;

		return true;
	}

	return false;
}


void Zombie::MoveTo()
{
	//===================================
	if (!SetRandPatrol) {
		pathfinder.UpdatePathFinder(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
		pathfinder.Run(path, 0);
	}
	cout << endl;

	// path값 전송
	Protocol::ZombiePath zPath;
	zPath.set_zombieid(ZombieData.zombieID);
	zPath.set_packet_type(10);

	for (const auto& p : path)
	{
		Protocol::Vector3* path = zPath.add_path();
		path->set_x(get<0>(p));
		path->set_y(get<1>(p));
		path->set_z(get<2>(p));
	}

	Protocol::Vector3* currentLocation = zPath.mutable_location();
	currentLocation->set_x(ZombieData.x);
	currentLocation->set_y(ZombieData.y);
	currentLocation->set_z(ZombieData.z);

	string serializedData;
	zPath.SerializeToString(&serializedData);

	for (const auto& player : g_players) {
		iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
	}


	//cout << endl;
	//if(path.size() != 0)
	//	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 경로의 첫 좌표: ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;

	//===================================

	cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;


	//좀비가 목적지에 도착하면
	if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {

		cout << "좀비 \'#" << ZombieData.zombieID << "\' 타겟 좌표 ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " ) 에 도착!!!" << endl;
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
		case TARGET::PATROL:
			//랜덤한 근처 장소로 이동하게 만들어서 배회 => 배회 중 목적지 닿으면 또 근처 장소 랜덤하게 타겟 잡아서 다시 이동
			SetRandPatrol = false;
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
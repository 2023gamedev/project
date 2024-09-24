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

	DistanceToPlayers = {};

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

	DistanceToPlayers = {};

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

// 플레이어를 단 한명이라도 포착해야 돌아감
void Zombie::SetDistance()
{ 
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB[bt_playerID].x, playerDB[bt_playerID].y, playerDB[bt_playerID].z}}};

	DistanceToPlayers.emplace(bt_playerID, sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2)));
}

bool Zombie::RandomPatrol()
{
	//cout << "RandomPatrol!!!" << endl;

	float px, py, pz;

	std::random_device rd;
	std::mt19937 mt(rd());
	
	std::uniform_int_distribution<int> dist(-1500, 1500);		//현 위치에서 반경 1500 +-

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

	float max = 0.f;
	vector<vector<vector<float>>> pl = {};
	vector<int> keys = {};

	// 단 한명의 플레이어라도 마주쳤다면
	if (DistanceToPlayers.size() != 0) {

		for (auto player : playerDB) {
			if (max < DistanceToPlayers.at(player.first)) {
				max = DistanceToPlayers.at(player.first);
				//pl = vector<vector<vector<float>>>{ {{player.second.x,player.second.y, player.second.z}} };
			}
		}

		// 같은 거리에 포착된 플레이어가 두명 이상일때, 그들중 랜덤한 플레이어 따라가게
		for (auto player : playerDB) {
			if (max == DistanceToPlayers.at(player.first)) {
				keys.emplace_back(player.first);
			}
		}

		std::random_device rd;
		std::mt19937 mt(rd());
		
		std::uniform_int_distribution<int> dist(0, keys.size() - 1);

		pl = vector<vector<vector<float>>>{ {{playerDB[keys[dist(mt)]].x,playerDB[keys[dist(mt)]].y, playerDB[keys[dist(mt)]].z}}};
	}

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

	Protocol::Zombie_attack attackpacket;

	attackpacket.set_zombieid(ZombieData.zombieID);
	attackpacket.set_packet_type(13);

	string serializedData;
	attackpacket.SerializeToString(&serializedData);

	for (const auto& player : g_players) {
		iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
	}


}


void Zombie::MoveTo(float deltasecond)
{
	// 최종 목표지점에 도착
	CheckFinalDestination();

	if (IsPathUpdated()) {
		cout << "PathUpdated!" << endl;
		ZombiePathIndex = 0;
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
		cout << "MOVETO ZOMBIE TYPE ERROR" << endl;
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

		// 경로의 끝에 도착
		if (ZombiePathIndex >= path.size()) {
			cout << "Zombie 경로 끝." << endl;
			ZombiePathIndex = 0;
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

		if (beforepath != path) {

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

void Zombie::CheckFinalDestination()
{
	//===================================
	if (!SetRandPatrol) {
		pathfinder.UpdatePathFinder(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
		pathfinder.Run(path, 0);
	}
	//cout << endl;

	//cout << endl;
	//if(path.size() != 0)
	//	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 경로의 첫 좌표: ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;

	//===================================

	/*cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;*/


	//좀비가 목적지에 도착하면
	if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {
		cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
		cout << endl;

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


void Zombie::SendPath()
{
	// 패트롤인 경우 RandPatrol에서 해당 작업을 이미 진행해주기 때문에 A* 중복 실행방지 
	if (!SetRandPatrol) {
		pathfinder.UpdatePathFinder(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
		pathfinder.Run(path, 0);
	}
	cout << endl;

	if (path.empty() || ZombiePathIndex >= path.size()) {
		return;
	}

	else {
		// path값 전송
		Protocol::ZombiePath zPath;
		zPath.set_zombieid(ZombieData.zombieID + 1);						// probuf 0번 인덱스 직렬화 불가를 보완하기위해서
		cout << "Path 보내는 좀비 ID: " << ZombieData.zombieID << endl;
		zPath.set_packet_type(10);

		// ================================== 전체 path 보내지 말고 ZombieIndex에 따라서 지금 이동해야할 목표점 좌표 하나만 뽑아서 보내기

		Protocol::Vector3* Destination = zPath.mutable_location();
		Destination->set_x(get<0>(path[ZombiePathIndex]));
		Destination->set_y(get<1>(path[ZombiePathIndex]));
		Destination->set_z(get<2>(path[ZombiePathIndex]));

		//cout << "x좌표 = " << get<0>(path[ZombiePathIndex]) << endl;
		//cout << "y좌표 = " << get<1>(path[ZombiePathIndex]) << endl;
		//cout << "z좌표 = " << get<2>(path[ZombiePathIndex]) << endl;

		string serializedData;
		zPath.SerializeToString(&serializedData);

		for (const auto& player : g_players) {
			iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
		}
	}


	//cout << endl;
	//if(path.size() != 0)
	//	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 경로의 첫 좌표: ( " << get<0>(path.front()) << ", " << get<1>(path.front()) << ", " << get<2>(path.front()) << " )" << endl;

	//===================================
}

void Zombie::Wait()
{
	//cout << "Zombie \'#" << ZombieData.zombieID << "\' wait for delay." << endl;
	//cout << endl;
}
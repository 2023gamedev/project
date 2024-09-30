#include <iostream>
#include <cmath>
#include <random>

#include "GStruct.pb.h"

#include "Zombie.h"
//#include "ZombiePathfinder.h"
#include "iocpServerClass.h"		// 전역변수 playerDB 사용하려구

using std::cout;
using std::endl;


Zombie::Zombie()
	: pathfinder(0,0,0,1,1,1)
{
	SetHP(0);

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

Zombie::Zombie(Zombie_Data z_d)
	: pathfinder(0, 0, 0, 1, 1, 1)
{
	SetHP(0);

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

Zombie::~Zombie()
{
	//여기 cout 해서 출력보면 왜 zombie 클래스가 생성도 되기 전인데 여러번 미리 불림?! 
	// -> 그래서 여기서 만약 new 할당된 메모리 delete 하려하면 할당된 메모리도 없는데 지울려 해서 에러 (지금은 다 없애서 없긴하지만... 미리 소멸자가 여러번 불리는 건 마찬가지)
	// -> 이유는 모르겠음 그냥
	// + 혹시 전방 선언해서?? / 복사 생성되서?? / 암시적 삭제??
}

void Zombie::SetDistance(int playerid)
{
	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB[playerid].x, playerDB[playerid].y, playerDB[playerid].z}} };

	float dist = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	if (DistanceToPlayers.find(playerid) == DistanceToPlayers.end()) {		// DistanceToPlayers map에 playerid가 없으면 -> 생성
		DistanceToPlayers.emplace(playerid, dist);
	}
	else {		// DistanceToPlayers map에 이미 playerid가 있으면 -> 수정
		DistanceToPlayers.at(playerid) = dist;
		//DistanceToPlayers[playerid] = dist;		// operator[] 이용해서 수정하기도 가능 
													// {주의} 이거 해당 키값이 없으면 자동으로 추가해주니까 조심해야함 (at은 해당 키값 없으면 abort()에러 띄움)
	}
}

void Zombie::RandomPatrol()
{
	cout << "New RandomPatrol!!!" << endl;

	float px, py, pz;

	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> dist(-1500, 1500);		//현 위치에서 반경 1500 +-

	px = ZombieData.x + dist(mt);
	py = ZombieData.y + dist(mt);
	pz = ZombieData.z;

	while (px >= 2366.f) {		// 좀비가 계단 쪽, 그리고 그쪽 벽 넘어로 자주 넘어가는 오류가 있어 이를 일단 방지하기 위해 (맵 수정 전까지는)
		px = ZombieData.x + dist(mt);

		if (ZombieData.x >= 2366.f) {
			cout << "[ERROR] 현재 좀비 걸을 수 있는 지형을 벗어남!!!" << endl;
			return;
		}
	}

	vector<tuple<float, float, float>> dest_test;

	// 랜덤 패트롤 지점이 갈 수 있는 지 검사
	CheckPath(dest_test, px, py, pz);

	UpdatePath(dest_test);
	
	cout << "TargetLocation[Patrol]: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;

	RandPatrolSet = true;
}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

	vector<vector<vector<float>>> pl = {};

	switch (targetType) {
	case TARGET::PLAYER:
		SearchClosestPlayer(pl);
		TargetLocation = pl;
		PrevTargetLocation = TargetLocation;
		//cout << "TargetLocation: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;
		break;
	case TARGET::SHOUTING:
		//==================================샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요
		break;
	case TARGET::FOOTSOUND:
		//TargetLocation = pl;			// => 이런식으로 하면 플레이어 최신 위치를 계속 갱신받게됨!
		break;
	case TARGET::INVESTIGATED:
		//TargetLocation = TargetLocation;		// 걍 명시적 표기 ========> [x]
												// 플레이어를 따라가던 도중 놓이면 따로 작업 할 꺼 없긴하지만 아니라면 (샤우팅/발소리->플레이어 이전위치) 이 상황에서는 갱신해줘야함
		TargetLocation = PrevTargetLocation;
		UpdatePath();							
		break;
	case TARGET::PATROL:
		if (RandPatrolSet == false) {
			RandomPatrol();
		}
		break;
	}
}

void Zombie::SearchClosestPlayer(vector<vector<vector<float>>>& closest_player)
{
	float min = FLT_MAX;	//float 최대값
	vector<int> keys = {};

	// 단 한명의 플레이어라도 마주쳤다면
	if (DistanceToPlayers.size() != 0) {

		for (auto player : playerDB) {
			if (DistanceToPlayers.find(player.first) != DistanceToPlayers.end()) {
				if (min > DistanceToPlayers.at(player.first) && DistanceToPlayers.at(player.first) > 0) {
					min = DistanceToPlayers.at(player.first);
				}
			}
		}

		// 같은 거리에 포착된 플레이어가 두명 이상일때, 그들중 랜덤한 플레이어 따라가게
		for (auto player : playerDB) {
			if (DistanceToPlayers.find(player.first) != DistanceToPlayers.end()) {
				if (min == DistanceToPlayers.at(player.first)) {
					keys.emplace_back(player.first);
					cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << player.first << " 을 따라감!!!" << endl;
					//cout << "플레이어 #" << player.first << " 의 위치: ( " << player.second.x << " , " << player.second.y << " , " << player.second.z << " )" << endl;
				}
			}
		}

		std::random_device rd;
		std::mt19937 mt(rd());

		std::uniform_int_distribution<int> dist(0, keys.size() - 1);

		// {주의} map 사용 할 때 주의할 점 (playerDB) => 이런식으로 사용하면 키값이 없을 경우 "새로 해당 키에 데이터는 없이" 데이터가 새로 추가가 됨!
		closest_player = vector<vector<vector<float>>>{ {{playerDB[keys[dist(mt)]].x, playerDB[keys[dist(mt)]].y, playerDB[keys[dist(mt)]].z}} };
	}
	else {	// (DistanceToPlayers.size() == 0)
		// BT에서 타겟을 플레이어로 했는데, DistanceToPlayers 맵이 비어 있다면 절대 안됨
		cout << "DistanceToPlayers Map ERROR!!! -> Target is set to Player but DistanceToPlayers Map is empty" << endl;
	}
}

void Zombie::Attack()
{
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어 \'#" << bt_playerID << "\' 을 공격하였습니다!" << endl;
	cout << endl;

	// 어택 통신 패킷 보내기

	Protocol::Zombie_attack attackpacket;

	attackpacket.set_zombieid(ZombieData.zombieID);
	attackpacket.set_packet_type(13);

	string serializedData;
	attackpacket.SerializeToString(&serializedData);

	for (const auto& player : g_players) {
		iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
	}

	IsAttacking = true;	// 좀비 공격중으로 변경

	HaveToWait = true;	// 좀비 BT 대기상태로 변경

	animStartTime = std::chrono::high_resolution_clock::now();		// 좀비 공격 시작 시간
}

void Zombie::MoveTo(float deltasecond)
{
	// 이미 최종 목표지점에 도착 -> 이제 BT 쓰레드에서 해당 함수 같이 돌리니, 해당 작업 필요 X (중복 작업)
	//if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1]) {
	//	cout << "MoveTo EQUAL" << endl;
	//	ReachFinalDestination();
	//	return;
	//}

	if (IsPathUpdated()) {
		ZombiePathIndex = 0;
	}

	if (ZombiePathIndex >= path.size()) {
		return;
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

		// 다음 목표 노드로 이동
		ZombiePathIndex++;

		// 경로의 끝에 도착 = 최종 목표지점에 도착
		if (ZombiePathIndex >= path.size()) {
			cout << "Zombie #" << ZombieData.zombieID << " 경로 끝 도착." << endl;
			ReachFinalDestination();
			ZombiePathIndex = 0;
		}
	}
	else {
		// 타겟 방향으로 이동
		ZombieData.x += moveX;
		ZombieData.y += moveY;
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

void Zombie::UpdatePath()
{
	pathfinder.UpdateStartGoal(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
	pathfinder.Run(path, 0);		// [+] 목표지점의 위치 변화가 없으면 A* 를 돌리지 않는 코드 작성되어 있음
	//cout << endl;
}

void Zombie::CheckPath(vector<tuple<float, float, float>>& goalTest_path, float goalTestX, float goalTestY, float goalTestZ)
{
	while (goalTest_path.empty() == true) {

		pathfinder.UpdateStartGoal(ZombieData.x, ZombieData.y, ZombieData.z, goalTestX, goalTestY, goalTestZ);
		pathfinder.Run(goalTest_path, 1);
		//cout << endl;

		if (goalTest_path.empty() == true) {
			cout << "랜덤 패트롤 지정 실패! 다시 검색!!!" << endl;
		}

	}

	goalTest_path.pop_back();	// 맨 마지막은 랜덤한 위치라서... 좀비가 걸을 수 있는 지형이 아닐 수 있음 -> 빼서 무조건 걸을 수 있는 위치가 최종 목적지!
}

void Zombie::UpdatePath(vector<tuple<float, float, float>> newPatrol_path)
{
	tuple<float, float, float> dest;
	if (!newPatrol_path.empty()) {
		dest = newPatrol_path.back();
	}

	TargetLocation[0][0][0] = get<0>(dest);
	TargetLocation[0][0][1] = get<1>(dest);
	TargetLocation[0][0][2] = ZombieData.z;

	path = newPatrol_path;
	// 참고로 pathfinder 변수의 start,goal 수정(UpdateStartGoal 함수 호출)은 CheckPath에서 미리 해줌
}

void Zombie::ReachFinalDestination()
{
	//UpdatePath();

	// 혹시 몰라서 한번 더 체크
	if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {
		//cout << "좀비 \'#" << ZombieData.zombieID << "\' 타겟 좌표[최종 목표 지점] ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " ) 에 도착!!!" << endl;
		//cout << endl;

		//<Selector Detect>의 Task들의 실행 조건이 되는 bool값들 초기화
		switch (targetType) {
		case TARGET::PLAYER:
			// 사실상 실행될 일 없음
			// 딱히 뭐 할 것도 없고;;
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
			RandPatrolSet = false;
			break;
		}

	}
}

void Zombie::SendPath()
{
	if (path.empty() || ZombiePathIndex >= path.size()) {
		return;
	}
	else {
		// path값 전송
		Protocol::ZombiePath zPath;
		zPath.set_zombieid(ZombieData.zombieID);
		zPath.set_packet_type(10);

		// ================================== 전체 path 보내지 말고 ZombieIndex에 따라서 지금 이동해야할 목표점 좌표 하나만 뽑아서 보내기

		Protocol::Vector3* Destination = zPath.mutable_path();
		Destination->set_x(get<0>(path[ZombiePathIndex]));
		Destination->set_y(get<1>(path[ZombiePathIndex]));
		Destination->set_z(get<2>(path[ZombiePathIndex]));

		Protocol::Vector3* currentLocation = zPath.mutable_location();
		currentLocation->set_x(ZombieData.x);
		currentLocation->set_y(ZombieData.y);
		currentLocation->set_z(ZombieData.z);

		string serializedData;
		zPath.SerializeToString(&serializedData);

		for (const auto& player : g_players) {
			iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
		}
	}

	if (targetType == TARGET::PLAYER) {
		cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 현재 경로의 바로 다음 좌표: ( " << get<0>(path[ZombiePathIndex]) << ", " << get<1>(path[ZombiePathIndex]) << ", " << get<2>(path[ZombiePathIndex]) << " )" << endl;
	}

	//cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 현재 경로의 바로 다음 좌표: ( " << get<0>(path[ZombiePathIndex]) << ", " << get<1>(path[ZombiePathIndex]) << ", " << get<2>(path[ZombiePathIndex]) << " )" << endl;

	/*cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;*/

}

void Zombie::Wait()
{
	//cout << "Zombie \'#" << ZombieData.zombieID << "\' wait for delay." << endl;
	//cout << endl;


	// 피격이 공격 보다 위에 => 공격 중에 피격 당하면 공격 캔슬되고 피격만 되게 하게 
	if (IsBeingAttacked) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

		cout << "Zombie #" << ZombieData.zombieID << " is being Attacked!" << endl;

		if (deltaTime.count() >= ZombieBeAttackedAnimDuration) {
			IsBeingAttacked = false;

			HaveToWait = false;

			IsAttacking = false;	// 혹시 공격중이다가 피격 당했을 경우를 대비해서 -> 리셋 개념
		}
		else {
			cout << "Attacked Animation time left " << ZombieBeAttackedAnimDuration - deltaTime.count() << "s" << endl;
		}
	}
	else if (IsAttacking) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

		cout << "Zombie #" << ZombieData.zombieID  << " is Attacking!" << endl;
		
		if (deltaTime.count() >= ZombieAttackAnimDuration) {
			IsAttacking = false;

			HaveToWait = false;
		}
		else {
			cout << "Attack Animation time left " << ZombieAttackAnimDuration - deltaTime.count() << "s" << endl;
		}
	}

	
}
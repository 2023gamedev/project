#include <iostream>
#include <cmath>
#include <random>

#include "GStruct.pb.h"

#include "Zombie.h"
//#include "ZombiePathfinder.h"
#include "iocpServerClass.h"		// 전역변수 playerDB_BT 사용하려구

using std::cout;
using std::endl;


Zombie::Zombie()
	: pathfinder(0,0,0,1,1,1)
{
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
}

Zombie::Zombie(Zombie_Data z_d)
	: pathfinder(0, 0, 0, 1, 1, 1)
{
	SetHP(0);

	SetSpeed(0);

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
}

Zombie::~Zombie()
{
	//여기 cout 해서 출력보면 왜 zombie 클래스가 생성도 되기 전인데 여러번 미리 불림?! 
	// -> 그래서 여기서 만약 new 할당된 메모리 delete 하려하면 할당된 메모리도 없는데 지울려 해서 에러 (지금은 다 없애서 없긴하지만... 미리 소멸자가 여러번 불리는 건 마찬가지)
	// -> 이유는 모르겠음 그냥
	// + 혹시 전방 선언해서?? / 복사 생성되서?? / 암시적 삭제??
}

void Zombie::DetermineFloor(float startZ)
{
	//cout << "좀비 시작 위치 - z 좌표: " << startZ << endl;

	if (startZ < 800.f) {
		z_floor = FLOOR::FLOOR_B2;
		//cout << "floor 할당: B2" << endl;
	}
	else if (startZ < 1800.f) {
		z_floor = FLOOR::FLOOR_B1;
		//cout << "floor 할당: B1" << endl;
	}
	else if (startZ < 2500.f) {
		z_floor = FLOOR::FLOOR_F1;
		//cout << "floor 할당: F1" << endl;
	}
	else if (startZ < 3600.f) {
		z_floor = FLOOR::FLOOR_F2;
		//cout << "floor 할당: F2" << endl;
	}
	else {
		z_floor = FLOOR::FLOOR_F3;
		//cout << "floor 할당: F3" << endl;
	}
}

// distanceType = 1: Detect / 2: FootSound
void Zombie::SetDistance(int playerid, int distanceType)
{
	map<int, float>* setMap = {};

	bool spacing = false;

	if (distanceType == 1)
		setMap = &DistanceTo_PlayerInsight;
	else if (distanceType == 2)
		setMap = &DistanceTo_FootSound;

	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB_BT[playerid].x, playerDB_BT[playerid].y, playerDB_BT[playerid].z}} };

	float dist = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));


	if (setMap->find(playerid) == setMap->end()) {		// map에 playerid가 없으면 -> 새로 생성
		setMap->emplace(playerid, dist);

#ifdef	ENABLE_BT_LOG
		if (distanceType == 1) {
			cout << "PlayerInsight 맵에 새로운 데이터 삽입! playerid: " << playerid << " , distance: " << dist << endl;
		}
		else {
			cout << "FootSound 맵에 새로운 데이터 삽입! playerid: " << playerid << " , distance: " << dist << endl;
		}
		spacing = true;
#endif
	}
	else if (setMap->find(playerid) != setMap->end()) {		// map에 이미 playerid가 이미 있으면 -> 수정, 갱신
		// PlayerInsight 업데이트
		if (distanceType == 1 /*&& setMap->at(playerid) != -1.0f*/) {	// 하지만 해당 값이 -1 (관측되지 않은 상태) 이면 수정/갱신 X -> 해당 조건식 검사할 필요X (데이터레이스는 이미 방지했고 수정이 필요한 놈만 해당 함수 부를 꺼니까)
			setMap->at(playerid) = dist;		// {주의} at은 해당 키값 없으면 abort() 에러 띄움 - 예외처리 꼭! 필요! (DistanceTo_PlayerInsight.find(playerid) == DistanceTo_PlayerInsight.end() -> 이거와 같이)
			//(*setMap)[playerid] = dist;		// operator[] 이용해서 수정하기도 가능 
												// {주의} 이거 해당 키값이 없으면 자동으로 추가해주니까 조심해야함 
		}
		// FootSound 업데이트
		else if (distanceType == 2) {
			//if (playerDB_BT[playerid].IsRunning == true) -> 굳이 검사해줄 필요 없음 (마찬가지로 업데이트가 필요한 놈만 부르니까)
			setMap->at(playerid) = dist;
			//else
			//	setMap->at(playerid) = -1.0f;	// FootSound_Update_Check에서 작업 처리함
		}
	}

#ifdef	ENABLE_BT_LOG
	if(spacing)
		cout << endl;
#endif
}

bool Zombie::RandomPatrol()
{
	//cout << "New RandomPatrol!!!" << endl;

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
			return false;
		}
	}

	while (py <= -1200.f) {		// 좀비가 화장실 뒤쪽 너머 빈 공간으로 자주 넘어가서 일단 막음
		py = ZombieData.y + dist(mt);

		if (ZombieData.x <= -1200.f) {
			cout << "[ERROR] 현재 좀비 걸을 수 있는 지형을 벗어남!!!" << endl;
			return false;
		}
	}

	vector<tuple<float, float, float>> dest_test;

	// 랜덤 패트롤 지점이 갈 수 있는 지 검사
	if (CheckPath(dest_test, px, py, pz) == false) {
		return false;
	}

	if (dest_test.size() < 3) {
		//cout << "좀비 #" << ZombieData.zombieID << " 새로운 패트롤 찾기 실패!!! => dest_test.size() < 3 -> 현재 지점과 너무 가까운 점" << endl;
		return false;
	}

	UpdatePath(dest_test);
	
#ifdef	ENABLE_BT_LOG
	cout << "랜덤 패트롤 찾기 성공!" << endl;
	cout << "TargetLocation[Patrol]: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;
#endif

	RandPatrolSet = true;

	return true;
}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

	vector<vector<vector<float>>> closest_player_pos = {};

	switch (targetType) {
	case TARGET::PLAYER:
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 의 목표 타겟: '플레이어'" << endl;
#endif
		SearchClosestPlayer(closest_player_pos, 1);
		if (closest_player_pos.size() != 0) {
			TargetLocation = closest_player_pos;
			PrevTargetLocation = TargetLocation;
		}
#ifdef	ENABLE_BT_LOG
		else {
			cout << "[Error] 좀비 따라갈 플레이어를 찾지못함! (closest_player_pos.size() == 0)" << endl;
		}
#endif
		break;
	case TARGET::SHOUTING:
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 의 목표 타겟: '샤우팅'" << endl;
#endif
		TargetLocation = ShoutingLocation;
		UpdatePath();
		break;
	case TARGET::FOOTSOUND:
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 의 목표 타겟: '발소리'" << endl;
#endif
		SearchClosestPlayer(closest_player_pos, 2);
		if (closest_player_pos.size() != 0) {
			TargetLocation = closest_player_pos;
			UpdatePath();
		}
#ifdef	ENABLE_BT_LOG
		else {
			cout << "[Error] 좀비 따라갈 발소리를 찾지못함! (closest_player_pos.size() == 0)" << endl;
		}
#endif
		break;
	case TARGET::INVESTIGATED:
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 의 목표 타겟: '이전 발견 위치'" << endl;
#endif
		if (TargetLocation != PrevTargetLocation && PrevTargetLocation.size() != 0) {	// 타겟위치가 재설정되지 않았다면, -> 반복 UpdatePath 하지 않도록
			TargetLocation = PrevTargetLocation;
			UpdatePath();
		}
		break;
	case TARGET::PATROL:
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 의 목표 타겟: '랜덤 패트롤'" << endl;
#endif
		int try_cnt = 0;
		if (RandPatrolSet == false) {
			bool result = false;
			while (result == false) {
				try_cnt++;
#ifdef	ENABLE_BT_LOG
				cout << "좀비 #" << ZombieData.zombieID << " 랜덤 패트롤 찾기 시도 - #" << try_cnt << endl;
#endif

				if (try_cnt >= 5) {					// 랜덤 패트롤 목표점 찾기 5번까지만 시도
//#ifdef	ENABLE_BT_LOG
					cout << "좀비 #" << ZombieData.zombieID << "랜덤 패트롤 찾기 결국 실패!!! (연속 5번 실패...) => 좀비 해당 한 틱동안 멍때림" << endl;
//#endif
					break;
				}

				result = RandomPatrol();
			}
		}
		break;
	}

#ifdef	ENABLE_BT_LOG
	cout << "TargetLocation: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;
#endif
}

// distanceType = 1: Detect / 2: FootSound
void Zombie::SearchClosestPlayer(vector<vector<vector<float>>>& closest_player_pos, int distanceType)
{
	float min = FLT_MAX;	//float 최대값

	map<int, float> searchMap = {};
	if (distanceType == 1)
		searchMap = DistanceTo_PlayerInsight;
	else if (distanceType == 2)
		searchMap = DistanceTo_FootSound;

	vector<int> closest_players = {};


	if (searchMap.size() != 0) {

		for (auto player : playerDB_BT) {
			// 죽은 플레이어 무시
			if (player.second.health <= 0) {
				continue;
			}
			// 연결 끊긴 플레이어 무시
			if (g_players.find(player.first) == g_players.end()) {
				continue;
			}

			if (searchMap.find(player.first) != searchMap.end()) {
				if (min > searchMap.at(player.first) && searchMap.at(player.first) > 0) {
					min = searchMap.at(player.first);
#ifdef	ENABLE_BT_LOG
					if(distanceType == 1)
						cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 시야에 존재함! --- 거리: " << searchMap.at(player.first) << endl;
					else if (distanceType == 2)
						cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 에게 발소리 들킴! --- 거리: " << searchMap.at(player.first) << endl;
#endif
				}
				else if (min > searchMap.at(player.first) && searchMap.at(player.first) <= 0) {
#ifdef	ENABLE_BT_LOG
					if (distanceType == 1)
						cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 시야에서 사라짐! --- 거리: " << searchMap.at(player.first) << endl;
					else if (distanceType == 2)
						cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 발소리 범위에서 사라짐! --- 거리: " << searchMap.at(player.first) << endl;
#endif
				}
			}
		}

		if (distanceType == 1 && PlayerInSight == false && min == FLT_MAX) {
			cout << "[ERROR] Data Race!!! -> PlayerInSight is false after CanSeePlayer Sequence check!" << endl;
			return;
		}

		// 혹시 같은 거리에 포착된 플레이어가 두명 이상일때, 그들중 랜덤한 플레이어 따라가게
		for (auto player : playerDB_BT) {
			// 죽은 플레이어 무시
			if (player.second.health <= 0) {
				continue;
			}
			// 연결 끊긴 플레이어 무시
			if (g_players.find(player.first) == g_players.end()) {
				continue;
			}

			if (searchMap.find(player.first) != searchMap.end()) {
				if (min == searchMap.at(player.first)) {
					closest_players.emplace_back(player.first);
#ifdef	ENABLE_BT_LOG
					cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 와 가장 가까움! --- 거리: " << searchMap.at(player.first) << endl;
					cout << "플레이어 #" << player.first << " 의 위치: ( " << player.second.x << " , " << player.second.y << " , " << player.second.z << " )" << endl;
#endif
				}
			}
		}

		if (closest_players.size() == 0) {
			if (distanceType == 1) {
				cout << "[ERROR] closest_players.size() == 0 -> 가장 가까운 플레이어(좀비시야) 찾을 수 없음" << endl;
			}
			else if (distanceType == 2) {
				cout << "[ERROR] closest_players.size() == 0 -> 가장 가까운 플레이어(발소리) 찾을 수 없음" << endl;
			}

			return;
		}

		std::random_device rd;
		std::mt19937 mt(rd());

		std::uniform_int_distribution<int> dist(0, closest_players.size() - 1);

		ClosestPlayerID = closest_players[dist(mt)];		// 가장 가까운 플레이어 인덱스 저장
		
		if (distanceType == 1) {
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << ClosestPlayerID << " 을 따라감!!!" << endl;
#endif
		}
		else if (distanceType == 2) {
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << ClosestPlayerID << " 의 발소리 따라감!!!" << endl;
#endif
		}

		// {주의} map 사용 할 때 주의할 점 (playerDB_BT[ClosestPlayerID].x) => 이런식으로 사용하면 키값이 없을 경우 "새로 해당 키에 실제 데이터는 없이" 더미 데이터가 새로 추가가 됨!
		closest_player_pos = vector<vector<vector<float>>>{ {{playerDB_BT[ClosestPlayerID].x, playerDB_BT[ClosestPlayerID].y, playerDB_BT[ClosestPlayerID].z}} };
	}
	else {	// (searchMap.size() == 0)
		if (distanceType == 1) {
			// BT에서 타겟을 플레이어로 했는데, DistanceTo_PlayerInsight 맵이 비어 있다면 절대 안됨
			cout << "DistanceTo_PlayerInsight Map \"ERROR\"!!! -> Target is set to Player but DistanceTo_PlayerInsight Map is empty" << endl;
		}
		else if (distanceType == 2) {
			cout << "DistanceTo_FootSound Map \"ERROR\"!!! -> Target is set to FootSound but DistanceTo_FootSound Map is empty" << endl;
		}
	}
}

void Zombie::Attack()
{
#ifdef	ENABLE_BT_LOG
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어 \'#" << ClosestPlayerID << "\' 을 공격하였습니다!" << endl;
	cout << endl;
#endif

	// 어택 통신 패킷 보내기

	Protocol::Zombie_attack attackpacket;

	attackpacket.set_zombieid(ZombieData.zombieID);
	attackpacket.set_playerid(ClosestPlayerID);
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
	bool spacing = false;

	//cout << "ZombiePathIndex: " << ZombiePathIndex << " , path.size(): " << path.size() << endl;
	//cout << endl;

	if (IsPathUpdated()) {
		ZombiePathIndex = 1;
	}

	if (ZombiePathIndex >= path.size() || ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 경로의 끝 도착. (before move)" << endl;
		cout << endl;
#endif
		ReachFinalDestination();
		ZombiePathIndex = 1;
		return;
	}

	// 현재 목표 노드
	tuple<float, float, float> TargetNode = path[ZombiePathIndex];
	float PathX = get<0>(TargetNode);
	float PathY = get<1>(TargetNode);

#ifdef	ENABLE_BT_LOG
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 현재 경로의 다음 좌표: ( " << get<0>(path[ZombiePathIndex]) << ", " << get<1>(path[ZombiePathIndex]) << ", " << get<2>(path[ZombiePathIndex]) << " )" << endl;
	spacing = true;
#endif

	if (PathX >= 2366.f) {	// 계단쪽 넘어로 이동하려하면
//#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 계단 넘어로 이동 방지로 인해 정지" << endl;
		//cout << endl;
//#endif
		ReachFinalDestination();
		ZombiePathIndex = 1;
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
		if (ZombiePathIndex >= path.size() || ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1]) {
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 경로의 끝 도착." << endl;
#endif
			ReachFinalDestination();
			ZombiePathIndex = 1;
		
			spacing = false;
		}
		else {	// 꼭지점을 넘어 갈 때
			float mid_deltasecond = newDistance / ZombieSpeed;
			float after_deltasecond = deltasecond - mid_deltasecond;

			spacing = false;

			MoveTo(after_deltasecond);
		}
	}
	else {
		// 타겟 방향으로 이동
		ZombieData.x += moveX;
		ZombieData.y += moveY;
	}

#ifdef	ENABLE_BT_LOG
	if (spacing)
		cout << endl;
#endif
}

bool Zombie::IsPathUpdated()
{
	if (!path.empty() && !beforepath.empty()) {

		if (beforepath != path) {
#ifdef	ENABLE_BT_LOG
			//cout << "IsPathUpdated(): (!path.empty() && !beforepath.empty()) && (beforepath != path) => return true" << endl;
			//cout << endl;
#endif
			beforepath = path;

			return true;
		}

	}

	if (!path.empty() && beforepath.empty()) {
#ifdef	ENABLE_BT_LOG
		//cout << "IsPathUpdated(): (!path.empty() && beforepath.empty()) => return true" << endl;
		//cout << endl;
#endif
		beforepath = path;

		return true;
	}

#ifdef	ENABLE_BT_LOG
	//cout << "IsPathUpdated() => return false" << endl;
	//cout << endl;
#endif

	return false;
}

void Zombie::UpdatePath()
{
	pathfinder.UpdateStartGoal(ZombieData.x, ZombieData.y, ZombieData.z, TargetLocation[0][0][0], TargetLocation[0][0][1], TargetLocation[0][0][2]);
	pathfinder.Run(path, 0);		// [+] 목표지점의 위치 변화가 없으면 A* 를 돌리지 않는 코드 작성되어 있음
	//cout << endl;
}

bool Zombie::CheckPath(vector<tuple<float, float, float>>& goalTest_path, float goalTestX, float goalTestY, float goalTestZ)
{
	pathfinder.UpdateStartGoal(ZombieData.x, ZombieData.y, ZombieData.z, goalTestX, goalTestY, goalTestZ);
	pathfinder.Run(goalTest_path, 1);
	//cout << endl;

	if (goalTest_path.empty() == true) {
//#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 랜덤 패트롤 지정 실패! (CheckPath 실패) 다시 검색!!!" << endl;
//#endif
		return false;
	}

	goalTest_path.pop_back();	// 맨 마지막은 랜덤한 위치라서... 좀비가 걸을 수 있는 지형이 아닐 수 있음 -> 빼서 무조건 걸을 수 있는 위치가 최종 목적지!

	return true;
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
	//if (ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {
#ifdef	ENABLE_BT_LOG
		cout << "좀비 '#" << ZombieData.zombieID << "' 타겟 좌표[최종 목표 지점] ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " ) 에 도착!!!" << endl;
#endif


		// BT 플래그 값 전체 초기화
		HeardShouting = false;
		HeardFootSound = false;
		KnewPlayerLocation = false;
		RandPatrolSet = false;


		//<Selector Detect>의 Task들의 실행 조건이 되는 bool값들 초기화
		switch (targetType) {
		case TARGET::PLAYER:
			// 사실상 실행될 일 없음
			// 딱히 뭐 할 것도 없고;;

#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 의 도달 타겟: '플레이어'" << endl;
#endif
			break;
		case TARGET::SHOUTING:
			//HeardShouting = false;
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 의 도달 타겟: '샤우팅'" << endl;
#endif
			break;
		case TARGET::FOOTSOUND:
			//HeardFootSound = false;
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 의 도달 타겟: '발소리'" << endl;
#endif
			break;
		case TARGET::INVESTIGATED:
			//KnewPlayerLocation = false;
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 의 도달 타겟: '이전 발견 위치'" << endl;
#endif
			break;
		case TARGET::PATROL:
			//랜덤한 근처 장소로 이동하게 만들어서 배회 => 배회 중 목적지 닿으면 또 근처 장소 랜덤하게 타겟 잡아서 다시 이동
			//RandPatrolSet = false;
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 의 도달 타겟: '랜덤 패트롤'" << endl;
#endif
			break;
		}

#ifdef	ENABLE_BT_LOG
		cout << endl;
#endif
	//}
}

void Zombie::SendPath()
{

	if (path.empty() || ZombiePathIndex >= path.size() || ZombieData.x == TargetLocation[0][0][0] && ZombieData.y == TargetLocation[0][0][1] /*&& ZombieData.z == TargetLocation[0][0][2]*/) {
		return;
	}
	else {
		// path값 전송
		Protocol::ZombiePath zPath;
		zPath.set_zombieid(ZombieData.zombieID);
		zPath.set_packet_type(10);

		Protocol::Vector3* Destination1 = zPath.mutable_path1();
		Destination1->set_x(get<0>(path[ZombiePathIndex]));
		Destination1->set_y(get<1>(path[ZombiePathIndex]));
		Destination1->set_z(get<2>(path[ZombiePathIndex]));

		if ((ZombiePathIndex + 1) < path.size()) {
			Protocol::Vector3* Destination2 = zPath.mutable_path2();
			Destination2->set_x(get<0>(path[ZombiePathIndex + 1]));
			Destination2->set_y(get<1>(path[ZombiePathIndex + 1]));
			Destination2->set_z(get<2>(path[ZombiePathIndex + 1]));
		}


		Protocol::Vector3* currentLocation = zPath.mutable_location();
		currentLocation->set_x(ZombieData.x);
		currentLocation->set_y(ZombieData.y);
		currentLocation->set_z(ZombieData.z);

		string serializedData;
		zPath.SerializeToString(&serializedData);


		//cout << "좀비 #" << ZombieData.zombieID << " 의 z_floor: " << z_floor << endl;
		
		for (const auto& player : playerDB_BT) {
			//cout << "playerDB_BT key: " << player.first << " 의 floor: " << player.second.floor << endl;
		
			if (z_floor == player.second.floor) {	// 클라 자기 층에 있는 좀비 정보만 받기 - 최적화 
				iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
		
				//cout << "(playerDB_BT key:)플레이어 #" << player.first << " SendPath 전송 완료 - 좀비 #" << ZombieData.zombieID << endl;
			}
		}
	}

	//if (targetType == TARGET::PLAYER) {
	//	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 현재 경로의 바로 다음 좌표: ( " << get<0>(path[ZombiePathIndex]) << ", " << get<1>(path[ZombiePathIndex]) << ", " << get<2>(path[ZombiePathIndex]) << " )" << endl;
	//}

	/*cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;*/

}

bool Zombie::PlayerInSight_Update_Check()
{
	bool result = false;

	bool spacing = false;


	// 시야에 있는 플레이어들 DistanceTo_PlayerInsight 맵 갱신
	for (auto player : playerDB_BT) {
		// 죽은 플레이어 무시
		if (player.second.health <= 0) {
			continue;
		}
		// 연결 끊긴 플레이어 무시
		if (g_players.find(player.first) == g_players.end()) {
			continue;
		}

		if (DistanceTo_PlayerInsight.find(player.first) != DistanceTo_PlayerInsight.end()) {	// 이미 DistanceTo_PlayerInsight에 저장된 값이 있다면
			if (DistanceTo_PlayerInsight.at(player.first) >= 0) {	// 그리고 포착 가능한 거리일 때
				SetDistance(player.first, 1);	// 거리 갱신하기
			}
		}
	}


	// 시야에 최소 한명이라도 플레이어가 있나 체크
	for (auto& distTo_playerinsight : DistanceTo_PlayerInsight) {
		if (distTo_playerinsight.second >= 0) {
			bool really_detected = true;

			if (playerDB_BT.find(distTo_playerinsight.first) != playerDB_BT.end()) {	// 아래 at 사용시에 혹시 모를 abort에러 방지용
				if (playerDB_BT.at(distTo_playerinsight.first).health <= 0) { // 플레이어가 이제 죽었다면
					distTo_playerinsight.second = -1.0f;	// 더이상 탐지 불가로 바꾸기
					really_detected = false;
				}
			}

			if (g_players.find(distTo_playerinsight.first) == g_players.end()) {	// 플레이어 연결이 이제 끊겼다면
				distTo_playerinsight.second = -1.0f;	// 더이상 탐지 불가로 바꾸기
				really_detected = false;
			}


			if (really_detected) {
#ifdef	ENABLE_BT_LOG
				cout << "좀비 #" << ZombieData.zombieID << " 시야에 플레이어 #" << distTo_playerinsight.first << " 를 확인! --- 거리: " << distTo_playerinsight.second << endl;
				spacing = true;
#endif

				result = true;
			}
		}
	}


#ifdef	ENABLE_BT_LOG
	if (result == false) {
		cout << "좀비 #" << ZombieData.zombieID << " 가 그 어떤 새로운 플레이어도 포착 못함! (PlayerInSight_Update_Check() == false)" << endl;
		spacing = true;
	}
#endif

#ifdef	ENABLE_BT_LOG
	if (spacing)
		cout << endl;
#endif

	PlayerInSight = result;		// PlayerInSight 검사는 FootSound와는 다르게 실시간으로 포착 가능상태일 때만 true여야함 (FootSound는 이전에 발소리를 들은 적이 있다면 해당 값 놔둬야하지만 -> false로 바꾸는 건 도착지에 도착하고 나서)

	return result;
}

bool Zombie::FootSound_Update_Check()
{
	bool result = false;

	bool spacing = false;


	// 뛰고 있는 플레이어들 DistanceTo_FootSound 맵에 갱신
	for (auto player : playerDB_BT) {
		// 죽은 플레이어 무시
		if (player.second.health <= 0) {
			continue;
		}
		// 연결 끊긴 플레이어 무시
		if (g_players.find(player.first) == g_players.end()) {
			continue;
		}

		if (player.second.IsRunning) {	// 일단 플레이어가 (거리 상관없이) 뛰었다면 FootSound map 접근
			SetDistance(player.first, 2);	// 갱신 (만약 없었을 시에는 새로 생성)
		}
		else {
			// 굳이 뛰고 있는 상태가 아니라면 FootSound map 건드릴 필요 없음
		}
	}


	// 좀비가 근처에서 발소리가 하나라도 났었는지 체크
	for (auto& distTo_footSound : DistanceTo_FootSound) {

		if (playerDB_BT.find(distTo_footSound.first) != playerDB_BT.end()) {	// 아래 at 사용시에 혹시 모를 abort에러 방지용
			if (playerDB_BT.at(distTo_footSound.first).health <= 0) { // 플레이어가 이제 죽었다면
				distTo_footSound.second = -1.0f;	// 더이상 탐지 불가로 바꾸기
				continue;
			}
		}

		if (g_players.find(distTo_footSound.first) == g_players.end()) {	// 플레이어 연결이 이제 끊겼다면
			distTo_footSound.second = -1.0f;	// 더이상 탐지 불가로 바꾸기
			continue;
		}


		if (distTo_footSound.second <= CanHearDistance && distTo_footSound.second > 0) {	// 발소리 포착 거리 내라면
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << distTo_footSound.first << " 의 새로운 발소리 포착함! --- 거리: " << distTo_footSound.second << endl;
			spacing = true;
#endif

			result = true;
		}
		else {
#ifdef	ENABLE_BT_LOG
			cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << distTo_footSound.first << " 의 새로운 발소리 포착 못함! --- 거리: " << distTo_footSound.second << endl;
			spacing = true;
#endif

			distTo_footSound.second = -1.0f;		//  발소리 탐지 거리 밖이면 -1 넣기
		}
	}


	if (result == false) {
#ifdef	ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 가 그 어떤 새로운 발소리도 포착 못함! (FootSound_Update_Check() == false)" << endl;
		spacing = true;
#endif
	}
	else {
		HeardFootSound = true;
	}

#ifdef	ENABLE_BT_LOG
	if (spacing)
		cout << endl;
#endif

	return result;
}

void Zombie::Wait()
{
	//cout << "좀비 '#" << ZombieData.zombieID << "' BT 잠시 대기." << endl;
	//cout << endl;


	// 피격이 공격 보다 위에 => 공격 중에 피격 당하면 공격 캔슬되고 피격만 되게 하게 
	if (IsBeingAttacked) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

#ifdef ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID << " 가 피격 당하였습니다!" << endl;
#endif

		if (deltaTime.count() >= ZombieBeAttackedAnimDuration) {
#ifdef ENABLE_BT_LOG
			cout << "피격 애니메이션 재생 끝 " << endl;
#endif

			IsBeingAttacked = false;

			HaveToWait = false;

			IsAttacking = false;	// 혹시 공격중이다가 피격 당했을 경우를 대비해서 -> 리셋 개념

			IsShouting = false;		// 마찬가지

			// 좀비 애니메이션 재생 후 순간이동하는 걸 막기위해
			MoveTo(IOCP_CORE::BT_deltaTime.count());
			//SendPath();		
		}
		else {
#ifdef ENABLE_BT_LOG
			cout << "피격 애니메이션 남은 시간: " << ZombieBeAttackedAnimDuration - deltaTime.count() << "s" << endl;
#endif
		}
	}
	else if (IsAttacking) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

#ifdef ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID  << " 가 공격 중입니다!" << endl;
#endif

		if (deltaTime.count() >= ZombieAttackAnimDuration) {
#ifdef ENABLE_BT_LOG
			cout << "공격 애니메이션 재생 끝 " << endl;
#endif

			IsAttacking = false;

			HaveToWait = false;

			IsShouting = false;		// 혹시 샤우팅중이다가 공격 할 경우를 대비해서 -> 리셋 개념
									//=> 사실 필요 없음 -> 샤우팅을 항상 먼저 할테고 그동안 공격은 못하므로

			// 좀비 애니메이션 재생 후 순간이동하는 걸 막기위해
			MoveTo(IOCP_CORE::BT_deltaTime.count());
			//SendPath();
		}
		else {
#ifdef ENABLE_BT_LOG
			cout << "공격 애니메이션 남은 시간: " << ZombieAttackAnimDuration - deltaTime.count() << "s" << endl;
#endif
		}
	}
	else if (IsShouting) {	// - 샤우팅 좀비 샤우팅 애니메이션

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

#ifdef ENABLE_BT_LOG
		cout << "좀비 #" << ZombieData.zombieID  << " 가 샤우팅 중입니다!" << endl;
#endif

		if (deltaTime.count() >= ZombieShoutingAnimDuration) {
#ifdef ENABLE_BT_LOG
			cout << "샤우팅 애니메이션 재생 끝 " << endl;
#endif

			IsShouting = false;

			HaveToWait = false;

			// 좀비 애니메이션 재생 후 순간이동하는 걸 막기위해
			MoveTo(IOCP_CORE::BT_deltaTime.count());
			//SendPath();
		}
		else {
#ifdef ENABLE_BT_LOG
			cout << "샤우팅 애니메이션 남은 시간: " << ZombieShoutingAnimDuration - deltaTime.count() << "s" << endl;
#endif
		}
	}

#ifdef ENABLE_BT_LOG
	cout << endl;
#endif
}
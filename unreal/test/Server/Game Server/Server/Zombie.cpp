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

// distanceType = 1: Detect , 2: FootSound / setTpye = 1: Insert(or Update) , 2: Update(no Insert)
void Zombie::SetDistance(int playerid, int distanceType, int setType)
{
	map<int, float>* setMap = {};

	if (distanceType == 1)
		setMap = &DistanceTo_PlayerInsight;
	else if (distanceType == 2)
		setMap = &DistanceTo_FootSound;

	vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
	vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{playerDB[playerid].x, playerDB[playerid].y, playerDB[playerid].z}} };

	float dist = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	if (setMap->find(playerid) == setMap->end() && setType == 1) {		// map에 playerid가 없고, setType != 2 -> 수정/갱신용이 아니라 setType == 1 -> 생성일때만 ==> 생성
		setMap->emplace(playerid, dist);
	}
	else if (setMap->find(playerid) != setMap->end()) {		// map에 이미 playerid가 있으면 -> 수정/갱신
		//if (setMap->at(playerid) != -1.0f) {				// 하지만 해당 값이 -1 (관측되지 않은 상태) 이면 수정/갱신 X
			if (distanceType == 1) {
				setMap->at(playerid) = dist;		// {주의} at은 해당 키값 없으면 abort() 에러 띄움 - 예외처리 꼭! 필요! (DistanceTo_PlayerInsight.find(playerid) == DistanceTo_PlayerInsight.end() -> 이거와 같이)
				//(*setMap)[playerid] = dist;		// operator[] 이용해서 수정하기도 가능 
													// {주의} 이거 해당 키값이 없으면 자동으로 추가해주니까 조심해야함 
			}

			else if (distanceType == 2) {
				if (playerDB[playerid].IsRunning == true)
					setMap->at(playerid) = dist;
				//else
				//	setMap->at(playerid) = -1.0f;
			}
		//}
	}
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
		cout << "CheckPath 실패!!!" << endl;
		return false;
	}

	if (dest_test.size() < 3) {
		cout << "새로운 패트롤 찾기 실패!!! => dest_test.size() < 3 -> 현재 지점과 너무 가까운 점" << endl;
		return false;
	}

	UpdatePath(dest_test);
	
	//cout << "랜덤 패트롤 찾기 성공!" << endl;
	//cout << "TargetLocation[Patrol]: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;

	RandPatrolSet = true;

	return true;
}

//void NewRandomPatrol()
//{
//	std::random_device rd;
//	std::mt19937 mt(rd());
//
//	std::uniform_int_distribution<int> dist_in(0, 5000);	
//	std::uniform_int_distribution<int> dist_over(0, 3000);
//
//	float r_x = dist_in(mt) - dist_over(mt);
//	
//}

void Zombie::SetTargetLocation(TARGET t)
{
	targetType = t;

	vector<vector<vector<float>>> closest_player_pos = {};

	switch (targetType) {
	case TARGET::PLAYER:
		SearchClosestPlayer(closest_player_pos, 1);
		if (closest_player_pos.size() != 0) {
			TargetLocation = closest_player_pos;
			PrevTargetLocation = TargetLocation;
		}
		//cout << "TargetLocation: ( " << TargetLocation[0][0][0] << " , " << TargetLocation[0][0][1] << " , " << TargetLocation[0][0][2] << " )" << endl;
		break;
	case TARGET::SHOUTING:
		//==================================샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요
		break;
	case TARGET::FOOTSOUND:
		//TargetLocation = pl;			// => 이런식으로 하면 플레이어 최신 위치를 계속 갱신받게됨!

		//if (HeardFootSound == false) {	// => 플레이어의 발소리를 들었을 당시 그 순간에 플레이어 위치를 받기
		//	SearchClosestPlayer(closest_player_pos, 2);
		//	HeardFootSound = true;
		//}
		//if (TargetLocation != closest_player_pos) {	// 타겟위치가 재설정되지 않았다면, -> 반복 UpdatePath 하지 않도록
		//	TargetLocation = closest_player_pos;
		//	UpdatePath();
		//}



		SearchClosestPlayer(closest_player_pos, 2);
		if (closest_player_pos.size() != 0) {
			TargetLocation = closest_player_pos;
		}
		HeardFootSound = true;
		UpdatePath();

		break;
	case TARGET::INVESTIGATED:
		//TargetLocation = TargetLocation;		// 걍 명시적 표기 ========> [x]
												// 플레이어를 따라가던 도중 놓이면 따로 작업 할 꺼 없긴하지만 아니라면 (샤우팅/발소리->플레이어 이전위치) 이 상황에서는 갱신해줘야함
												// 근데 위에 처럼 하면 발소리 듣고 발소리 쪽으로 갔다가 (경로 끝까지 가고) 다시 플레이어를 마지막으로 본위치로 가는게 약간 이상한 거 같음;; (회의 필요)
		if (TargetLocation != PrevTargetLocation && PrevTargetLocation.size() != 0) {	// 타겟위치가 재설정되지 않았다면, -> 반복 UpdatePath 하지 않도록
			TargetLocation = PrevTargetLocation;
			UpdatePath();
		}
		break;
	case TARGET::PATROL:
		int try_cnt = 0;
		if (RandPatrolSet == false) {
			while (RandomPatrol() == false) {
				try_cnt++;
				cout << "랜덤 패트롤 찾기 시도 #" << try_cnt << endl;

				if (try_cnt >= 5) {					// 랜덤 패트롤 목표점 찾기 5번까지만 시도
					cout << "랜덤 패트롤 찾기 결국 실패!!!" << endl;
				}
			}
		}
		break;
	}
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

		for (auto player : playerDB) {
			if (searchMap.find(player.first) != searchMap.end()) {
				if (min > searchMap.at(player.first) && searchMap.at(player.first) > 0) {
					min = searchMap.at(player.first);
					//if(distanceType == 1)
					//	cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 시야에 존재함! --- 거리: " << searchMap.at(player.first) << endl;
					//else if (distanceType == 2)
					//	cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 에게 발소리 들킴! --- 거리: " << searchMap.at(player.first) << endl;
				}
				else if (min > searchMap.at(player.first) && searchMap.at(player.first) <= 0) {
					//if (distanceType == 1)
					//	cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 시야에서 사라짐! --- 거리: " << searchMap.at(player.first) << endl;
					//else if (distanceType == 2)
					//	cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 의 발소리 범위에서 사라짐! --- 거리: " << searchMap.at(player.first) << endl;
				}
			}
		}

		if (distanceType == 1 && PlayerInSight == false && min == FLT_MAX) {
			cout << "[ERROR] Data Race!!! -> PlayerInSight is false after CanSeePlayer Sequence check!" << endl;
			return;
		}

		// 같은 거리에 포착된 플레이어가 두명 이상일때, 그들중 랜덤한 플레이어 따라가게
		for (auto player : playerDB) {
			if (searchMap.find(player.first) != searchMap.end()) {
				if (min == searchMap.at(player.first)) {
					closest_players.emplace_back(player.first);
					//cout << "플레이어 #" << player.first << " 가 좀비 #" << ZombieData.zombieID << " 와 가장 가까움! --- 거리: " << searchMap.at(player.first) << endl;
					//cout << "플레이어 #" << player.first << " 의 위치: ( " << player.second.x << " , " << player.second.y << " , " << player.second.z << " )" << endl;
				}
			}
		}

		if (closest_players.size() == 0) {
			if(distanceType == 1)
				cout << "DistanceTo_PlayerInSight is empty" << endl;
			else if(distanceType == 2)
				cout << "DistanceTo_FootSound is empty" << endl;
			return;
		}

		std::random_device rd;
		std::mt19937 mt(rd());

		std::uniform_int_distribution<int> dist(0, closest_players.size() - 1);

		ClosestPlayerID = closest_players[dist(mt)];		// 가장 가까운 플레이어 인덱스 저장
		
		if (distanceType == 1) {
			//cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << ClosestPlayerID << " 을 따라감!!!" << endl;
		}
		else if (distanceType == 2) {
			//cout << "좀비 #" << ZombieData.zombieID << " 가 플레이어 #" << ClosestPlayerID << " 의 발소리 따라감!!!" << endl;
		}

		// {주의} map 사용 할 때 주의할 점 (playerDB) => 이런식으로 사용하면 키값이 없을 경우 "새로 해당 키에 데이터는 없이" 데이터가 새로 추가가 됨!
		closest_player_pos = vector<vector<vector<float>>>{ {{playerDB[ClosestPlayerID].x, playerDB[ClosestPlayerID].y, playerDB[ClosestPlayerID].z}} };
	}
	else {	// (searchMap.size() == 0)
		if (distanceType == 1) {
			// BT에서 타겟을 플레이어로 했는데, DistanceTo_PlayerInsight 맵이 비어 있다면 절대 안됨
			cout << "DistanceTo_PlayerInsight Map ERROR!!! -> Target is set to Player but DistanceTo_PlayerInsight Map is empty" << endl;
		}
		else if (distanceType == 2) {
			cout << "DistanceTo_FootSound Map ERROR!!! -> Target is set to FootSound but DistanceTo_FootSound Map is empty" << endl;
		}
	}
}

void Zombie::Attack()
{
	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 플레이어 \'#" << ClosestPlayerID << "\' 을 공격하였습니다!" << endl;
	//cout << endl;

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
	if (IsPathUpdated()) {
		ZombiePathIndex = 1;
	}

	if (ZombiePathIndex >= path.size()) {
		return;
	}

	// 현재 목표 노드
	tuple<float, float, float> TargetNode = path[ZombiePathIndex];
	float PathX = get<0>(TargetNode);
	float PathY = get<1>(TargetNode);

	if (ZombieData.zombietype == 0) {
		//ZombieSpeed = 200.f;
	}
	else if (ZombieData.zombietype == 1) {
		//ZombieSpeed = 300.f;
	}
	else if (ZombieData.zombietype == 2) {
		//ZombieSpeed = 400.f;
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
			//cout << "Zombie #" << ZombieData.zombieID << " 경로 끝 도착." << endl;
			ReachFinalDestination();
			ZombiePathIndex = 1;
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

bool Zombie::CheckPath(vector<tuple<float, float, float>>& goalTest_path, float goalTestX, float goalTestY, float goalTestZ)
{
	pathfinder.UpdateStartGoal(ZombieData.x, ZombieData.y, ZombieData.z, goalTestX, goalTestY, goalTestZ);
	pathfinder.Run(goalTest_path, 1);
	//cout << endl;

	if (goalTest_path.empty() == true) {

		cout << "좀비 #" << ZombieData.zombieID << " 랜덤 패트롤 지정 실패! 다시 검색!!!" << endl;
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
			if (pathfinder.floor == playerDB[player.first].floor) {
				iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());

				cout << "SendPath 전송 완료 - 좀비 #" << ZombieData.zombieID << endl;
			}
		}

	}

	//if (targetType == TARGET::PLAYER) {
	//	cout << "좀비 \'#" << ZombieData.zombieID << "\' 가 이동 해야할 현재 경로의 바로 다음 좌표: ( " << get<0>(path[ZombiePathIndex]) << ", " << get<1>(path[ZombiePathIndex]) << ", " << get<2>(path[ZombiePathIndex]) << " )" << endl;
	//}

	/*cout << "좀비 \'#" << ZombieData.zombieID << "\' 의 타겟 좌표[최종 목표 지점]: ( " << TargetLocation[0][0][0] << ", " << TargetLocation[0][0][1] << ", " << TargetLocation[0][0][2] << " )" << endl;
	cout << endl;*/

}

bool Zombie::FootSoundCheck() 
{
	bool result = false;

	// 뛰고 있는 플레이어들 DistanceTo_FootSound 맵에 저장
	for (auto player : playerDB) {
		if (player.second.IsRunning) {
			SetDistance(player.first, 2, 1);
			result = true;
		}
		else {
			//SetDistance(player.first, 2, 1);
		}
	}

	if (result == false) {
		return false;
	}


	// 발소리를 들을 수 있는 거리에 좀비가 있었는지 체크
	result = false;
	
	for (auto& distTo_footSound : DistanceTo_FootSound) {
		if (distTo_footSound.second <= CanHearDistance) { 
			result = true;
		}
		else {
			distTo_footSound.second = -1.0f;		// 발소리 탐지 거리 밖이면 -1 넣기
		}
	}

	return result;
}

void Zombie::Wait()
{
	//cout << "Zombie \'#" << ZombieData.zombieID << "\' wait for delay." << endl;
	//cout << endl;


	// 피격이 공격 보다 위에 => 공격 중에 피격 당하면 공격 캔슬되고 피격만 되게 하게 
	if (IsBeingAttacked) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

		//cout << "Zombie #" << ZombieData.zombieID << " is being Attacked!" << endl;

		if (deltaTime.count() >= ZombieBeAttackedAnimDuration) {
			IsBeingAttacked = false;

			HaveToWait = false;

			IsAttacking = false;	// 혹시 공격중이다가 피격 당했을 경우를 대비해서 -> 리셋 개념

			// 좀비 애니메이션 재생 후 순간이동하는 걸 막기위해
			MoveTo(IOCP_CORE::BT_deltaTime.count());
			SendPath();		
		}
		else {
			//cout << "Attacked Animation time left " << ZombieBeAttackedAnimDuration - deltaTime.count() << "s" << endl;
		}
	}
	else if (IsAttacking) {

		auto waitAfterTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = waitAfterTime - animStartTime;

		//cout << "Zombie #" << ZombieData.zombieID  << " is Attacking!" << endl;
		
		if (deltaTime.count() >= ZombieAttackAnimDuration) {
			IsAttacking = false;

			HaveToWait = false;

			// 좀비 애니메이션 재생 후 순간이동하는 걸 막기위해
			MoveTo(IOCP_CORE::BT_deltaTime.count());
			SendPath();
		}
		else {
			//cout << "Attack Animation time left " << ZombieAttackAnimDuration - deltaTime.count() << "s" << endl;
		}
	}
	
}
#pragma once

#include <vector>
#include <string>
#include <map>
#include <chrono>

#include "Task.h"
//#include "Player.h"
#include "ZombiePathfinder.h"

using std::vector;
using std::string;
using std::tuple;
using std::map;

class Task;
//class Player;
class IOCP_CORE;           
class ZombiePathfinder;     // 전방 선언 (순환 포함 문제를 피하기 위해)

// 통신에서 주로 사용할 데이터 클래스
class Zombie_Data {
public:
    int zombieID;
    float x, y, z;
    float pitch, yaw, roll;
    int zombietype;             // 0 - 일반 좀비, 1 - 샤우팅 좀비, 2 - 뛰는 좀비

    // 사실상 밑에 두개는 사용 X (현재는)
    int patroltype;
    float patrolrange;

    Zombie_Data() = default;
    Zombie_Data(int zid, float x, float y, float z, float pitch, float yaw, float roll, int ztype, int pattype, float patrange) 
        : zombieID(zid), x(x), y(y), z(z), pitch(pitch), yaw(yaw), roll(roll), zombietype(ztype), patroltype(pattype), patrolrange(patrange) {} //초기화 리스트
};

class Zombie {
public:
    enum TARGET {
        NULL_TARGET,
        PLAYER,
        SHOUTING,
        FOOTSOUND,
        INVESTIGATED,
        PATROL             
    };


    IOCP_CORE* iocpServer;


    vector<tuple<float, float, float>> path;        //PathFinder로부터 받을 경로 좌표값들 저장
    vector<tuple<float, float, float>> beforepath;  //PathFinder로부터 받은 이전 경로 저장


    const float CanAttackDistance = 150.f;          // 공격 사정거리 150.f

    const float CanHearDistance = 500.f;            // 발소리 포착 가능거리500.f

    //const float CanHearShoutDistance = 2000.f;      //========================언리얼 BaseZombie.cpp에서 Shouting() - float DetectRadius = 2000.f;에서 참조 (맞는지 확인 필요)

    const float ZombieAttackAnimDuration = 2.63f;    // 좀비 공격 애니메이션 재생 시간 (* 정확히는 2.63초)

    const float ZombieBeAttackedAnimDuration = 2.0f;    // 좀비 피격 애니메이션 재생 시간 (* 정확히는 2.00초)

    const float NormalZombieStartHP = 20.0f;        // 20.0f

    Zombie_Data ZombieData;     // 통신에서 주로 사용할 데이터

    ZombiePathfinder pathfinder;
    
    map<int, float> DistanceTo_PlayerInsight;     // Detect용 맵 <플레이어 인덱스, 좀비-플레이어 거리>

    map<int, float> DistanceTo_FootSound;         // FootSound용 맵 <플레이어 인덱스, 좀비-플레이어 거리>

    vector<vector<vector<float>>> TargetLocation;

    vector<vector<vector<float>>> PrevTargetLocation;       // 플레이어를 마지막으로 본 위치

    int ClosestPlayerID;

    bool PlayerInSight;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    bool RandPatrolSet;     // 랜덤 패트롤 지점이 set 되면 true

    bool IsAttacking;       // 해당 좀비 지금 공격 중인가? (애니메이션 재생 중인 가?)

    bool IsBeingAttacked;   // 해당 좀비 지금 피격 당하는 중인가? (애니메이션 재생 중인 가?)

    bool HaveToWait;        // BT가 대기상태를 해야 하는지 판별

    std::chrono::steady_clock::time_point animStartTime;      // 좀비 애니메이션 시작 시간
    
    //float speed;

    TARGET targetType;

    int ZombiePathIndex = 0;

    float zombieHP;


    Zombie();

    Zombie(Zombie_Data zd);

    ~Zombie();


    void SetDistance(int playerid, int distanceType, int setType);    // distanceType = 1: Detect / 2: FootSound, setTpye = 1: Insert / 2: Update

    void SetTargetLocation(TARGET t);

    void Attack() ;

    bool FootSoundCheck();

    void SendPath();

    void Wait();

    void MoveTo(float deltasecond);

    bool IsPathUpdated();

    bool RandomPatrol();

    void ReachFinalDestination();

    void UpdatePath();
    // 얜 패트롤용
    void UpdatePath(vector<tuple<float, float, float>> newPatrol_path); 

    bool CheckPath(vector<tuple<float, float, float>>& goalTest_path, float goalTestX, float goalTestY, float goalTestZ);

    void SearchClosestPlayer(vector<vector<vector<float>>>& closest_player_pos, int distanceType);

    virtual float GetHP() const { return zombieHP;  }
    virtual void SetHP(float hp) { zombieHP = hp; zombieHP = hp; }

    //void ShoutingHear();
    
    //void FootSoundHear();
};
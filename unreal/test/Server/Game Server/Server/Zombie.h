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

    //const float CanHearDistance = 500.f;            //========================언리얼 BaseCharacter.cpp에서 FootSound() - float DetectRadius = 500.f;에서 참조 (맞는지 확인 필요)
    //const float CanHearShoutDistance = 2000.f;      //========================언리얼 BaseZombie.cpp에서 Shouting() - float DetectRadius = 2000.f;에서 참조 (맞는지 확인 필요)

    const float ZombieAttackAnimDuration = 2.7f;    // 좀비 공격 애니메이션 재생 시간 (* 정확히는 2.63초)

    Zombie_Data ZombieData;     // 통신에서 주로 사용할 데이터

    ZombiePathfinder pathfinder;
    
    map<int, float> DistanceToPlayers;

    vector<vector<vector<float>>> TargetLocation;

    vector<vector<vector<float>>> PrevTargetLocation;

    bool PlayerInSight;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    bool RandPatrolSet;     // 랜덤 패트롤 지점이 set 되면 true

    bool IsAttacking;       // 해당 좀비 지금 공격 중인가? (애니메이션 재생 중인 가?)

    bool HaveToWait;        // BT가 대기상태를 해야 하는지 판별

    std::chrono::steady_clock::time_point attackAnimStartTime;      // 좀비 공격 시작 시간
    
    //float speed;

    TARGET targetType;

    int bt_playerID;        //BT 돌릴때, 어떤 플레이어와 검사 할지를 설정하기 위해 사용 -> SetTargetLocation 등에서 필요

    int ZombiePathIndex = 0;

    float zombieHP;


    Zombie();

    Zombie(Zombie_Data zd);

    ~Zombie();

    // 대입 연산자 오버로딩
    //Zombie& operator=(const Zombie& from) {
    //    cout << "Zombie& operator=(const Zombie& from)" << endl;
    //    ZombieData = from.ZombieData;
    //    zombieHP = from.zombieHP;
    //    return *this;
    //}


    void SetDistance(int playerid);

    void SetTargetLocation(TARGET t);

    void Attack() ;

    void SendPath();

    void Wait();

    void MoveTo(float deltasecond);

    bool IsPathUpdated();

    void RandomPatrol();

    void ReachFinalDestination();

    void UpdatePath();
    // 얜 패트롤용
    void UpdatePath(vector<tuple<float, float, float>> newPatrol_path); 

    void CheckPath(vector<tuple<float, float, float>>& goalTest_path, float goalTestX, float goalTestY, float goalTestZ);

    void SearchClosestPlayer(vector<vector<vector<float>>>& closest_player);

    virtual float GetHP() const { cout << "Zombie - GetHP()" << endl; return zombieHP;  }
    virtual void SetHP(float hp) { zombieHP = hp; zombieHP = hp; cout << "Zombie - SetHP(float hp)" << endl; }

    //void ShoutingHear();
    
    //void FootSoundHear();
};
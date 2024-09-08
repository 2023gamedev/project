#pragma once

#include <vector>
#include <string>

#include "Task.h"
#include "Player.h"
#include "iocpServerClass.h"

using std::vector;
using std::string;
using std::tuple;


class Task;
class Player;
class IOCP_CORE;

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
        PLAYER,
        SHOUTING,
        FOOTSOUND,
        INVESTIGATED,
        ORIGIN             
    };


    IOCP_CORE* iocpServer;


    vector<tuple<float, float, float>> path;        //PathFinder로부터 받을 경로 좌표값들 저장
    vector<tuple<float, float, float>> beforepath;  //PathFinder로부터 받은 이전 경로 저장


    const float CanAttackDistance = 150.f;          //========================언리얼 BTIsinAttackRange.cpp에서 CalculateRawConditionValue(...) - if (DistanceToTarget > 150.f)에서 참조 (맞는지 확인 필요)

    const float CanHearDistance = 500.f;            //========================언리얼 BaseCharacter.cpp에서 FootSound() - float DetectRadius = 500.f;에서 참조 (맞는지 확인 필요)

    const float CanHearShoutDistance = 2000.f;      //========================언리얼 BaseZombie.cpp에서 Shouting() - float DetectRadius = 2000.f;에서 참조 (맞는지 확인 필요)

    Zombie_Data ZombieData;     // 통신에서 주로 사용할 데이터

    vector<vector<vector<float>>> ZombieOriginLocation;   //private 선언하는 것도 좋을 듯 (상수로 사용하니) - const는 생각해보면 불가능함

    float DistanceToPlayer;

    vector<vector<vector<float>>> TargetLocation;

    bool PlayerInSight;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    float speed;

    TARGET targetType;

    int bt_playerID;        //BT 돌릴때, 어떤 플레이어와 검사 할지를 설정하기 위해 사용 -> SetTargetLocation 등에서 필요

    int ZombiePathIndex = 0;

    Zombie();

    Zombie(Zombie_Data zd, vector<vector<vector<float>>> zl);

    ~Zombie();


    void SetDistance();

    void SetTargetLocation(TARGET t);

    void Attack() ;

    void MoveTo();

    void Wait();

    void Walk(float deltasecond);

    bool IsPathUpdated();

    bool RandomPatrol();

    //void ShoutingHear();
    
    //void FootSoundHear();
};
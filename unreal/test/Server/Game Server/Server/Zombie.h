#pragma once

#include <vector>
#include <string>

#include "Task.h"
#include "Player.h"

using std::vector;
using std::string;
using std::tuple;


class Task;
class Player;

// 통신에서 주로 사용할 데이터 클래스
class Zombie_Data {
public:
    int zombieID;
    float x, y, z;
    float pitch, yaw, roll;
    int zombietype;
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

    
    vector<tuple<float, float, float>> path;        //PathFinder로부터 받을 경로 좌표값들 저장


    const float CanAttackDistance = 10.f;

    const float CanHearDistance = 30.f;

    Zombie_Data ZombieData;     // 통신에서 주로 사용할 데이터

    vector<vector<vector<float>>> ZombieOriginLocation;   //private 선언하는 것도 좋을 듯 (상수로 사용하니) - const는 생각해보면 불가능함

    float DistanceToPlayer;

    vector<vector<vector<float>>> TargetLocation;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    float speed;

    TARGET targetType;

    int bt_playerID;        //BT 돌릴때, 어떤 플레이어와 검사 할지를 설정하기 위해 사용 -> SetTargetLocation 등에서 필요


    Zombie();

    Zombie(Zombie_Data zd, vector<vector<vector<float>>> zl);

    ~Zombie();


    void SetDistance();

    void SetTargetLocation(TARGET t);

    void Attack() ;

    void MoveTo();

    void Wait();

    void Walk();

    //void ShoutingHear();
    
    //void FootSoundHear();
};
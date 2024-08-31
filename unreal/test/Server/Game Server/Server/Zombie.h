#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Task.h"
#include "Player.h"
#include "iocpServerClass.h"

using namespace std;


class Task;
class Player;
class IOCP_CORE;

struct Zombie_Data {
    int zombieID;
    float x, y, z;
    float pitch, yaw, roll;
    int zombietype;
    int patroltype;
    float patrolrange;

    Zombie_Data() = default;
    Zombie_Data(int zid, float x, float y, float z, float pitch, float yaw, float roll, int ztype, int pattype, float patrange) 
        : zombieID(zid), x(x), y(y), z(z), pitch(pitch), yaw(yaw), roll(roll), zombietype(ztype), patroltype(pattype), patrolrange(patrange) {}
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
                                            

    Player* PL; //일단 싱글 플레이어 게임으로 산정 => 플레이어들 정보 변수 iocpServerClass.h 에 전역으로 선언 되있음 => 나중에 삭제하고 코드들 수정 몇개 해야함

    
    vector<tuple<float, float, float>> path;        //PathFinder로부터 받을 경로 좌표값들 저장


    const float CanAttackDistance = 10.f;

    const float CanHearDistance = 30.f;

    Zombie_Data ZombieData;     // 통신에서 주로 사용할 데이타들 구조체

    vector<vector<vector<float>>> ZombieOriginLocation;   //private 선언하는 것도 좋을 듯 (상수로 사용하니) - const는 생각해보면 불가능함

    float DistanceToPlayer;

    vector<vector<vector<float>>> TargetLocation;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    int speed;

    TARGET targetType;


    Zombie();

    Zombie(Zombie_Data zd, Player* p, vector<vector<vector<float>>> zl);

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
#pragma once

#include <vector>
#include <string>
#include <memory>

#include "Task.h"
#include "Player.h"

using namespace std;


class Task;
class Player;

class Zombie {
public:
    enum TARGET {
        PLAYER,
        SHOUTING,
        FOOTSOUND,
        INVESTIGATED,
        ORIGIN
    };

 
    vector<unique_ptr<Task>>* Z_BT;         //포인터로 사용 안하면 (C2280: 삭제된 함수를 참조하려고 합니다.) 에러 발생 - [근데, 굳이 unique_ptr을 사용해야 할...까? 고민]
                                            

    Player* PL; //일단 싱글 플레이어으로 산정


    const float CanSeeDistance = 500000.f;      //나중에 지우기

    const float CanAttackDistance = 10.f;

    const float CanHearDistance = 30.f;


    string name;

    vector<vector<vector<float>>> ZombieLocation;

    vector<vector<vector<float>>> ZombieOriginLocation;   //private 선언하는 것도 좋을 듯 (상수로 사용하니) - const는 생각해보면 불가능함

    float DistanceToPlayer;

    vector<vector<vector<float>>> TargetLocation;

    bool KnewPlayerLocation;
  
    bool HeardShouting;

    bool HeardFootSound;

    int speed;

    TARGET targetType;


    Zombie();

    Zombie(Player* p, string n, vector<vector<vector<float>>> zl);


    void SetDistance();

    void SetTargetLocation(TARGET t);

    void Attack() ;

    void MoveTo();

    void Wait();

    //void ShoutingHear();
    
    //void FootSoundHear();
};
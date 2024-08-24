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


    vector<unique_ptr<Task>>* Z_BT;         //포인터로 사용 안하면 (C2280: 삭제된 함수를 참조하려고 합니다.) 에러 발생 - 근데 Sequence랑 Selector에서 사용하는 건 멀쩡;;

    Player* PL; //일단 싱글 플레이어으로 산정


    const float CanSeeDistance = 50.f;

    const float CanAttackDistance = 10.f;

    const float CanHearDistance = 30.f;


    string name;

    vector<vector<vector<int>>> ZombieLocation;

    vector<vector<vector<int>>> ZombieOriginLocation;   //private 선언하는 것도 좋을 듯 (상수로 사용하니)

    float DistanceToPlayer;

    vector<vector<vector<int>>> TargetLocation;

    bool KnewPlayerLocation;
    
    bool HeardFootSound;
   
    bool HeardShouting;


    Zombie();

    Zombie(Player* p, string n, vector<vector<vector<int>>> zl);


    void SetDistance();

    void SetTargetLocation(TARGET t);

    void Attack() const;

    void MoveTo() const;

    void Wait() const;
};
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


    vector<unique_ptr<Task>>* Z_BT;

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
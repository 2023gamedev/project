#pragma once

#include <vector>
#include <string>

#include "Task.h"

using namespace std;

class Task;

class Zombie {
public:
    Task* Z_BT;


    const float CanSeeDistance = 50.f;

    const float CanAttackDistance = 10.f;

    string name;

    float DistanceToPlayer;

    vector<vector<vector<int>>> TargetLocation;

    bool KnewPlayerLocation;
    
    bool HeardFootSound;
   
    bool HeardShouting;


    Zombie();

    Zombie(string n, float dtp, vector<vector<vector<int>>> tl, bool kpl, bool hfs, bool hs);


    void SetDistance(float dtp);

    void SetPlayerLocation(vector<vector<vector<int>>> pll);

    void Attack() const;

    void MoveTo() const;

    void Wait() const;
};
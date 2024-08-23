#pragma once

#include <vector>

using namespace std;

class Zombie {
public:
    static float DistanceToPlayer;

    static const float CanSeeDistance;

    static const float CanAttackDistance;

    static vector<vector<vector<int>>> PlayerLastLocation;

    static bool Investigated;
    
    static bool HeardFootSound;
   
    static bool HeardShouting;


    void SetDistance(float dtp);

    void SetPlayerLocation(vector<vector<vector<int>>> pll);

};
#pragma once

#include <set>
#include <fstream>
#include <sstream>
#include <string>

#include "Node.h"

using namespace std;


enum class FLOOR {
    FLOOR_B2,
    FLOOR_B1,
    FLOOR_F1,
    FLOOR_F2,
    FLOOR_F3
};

struct TupleHash {
    size_t operator()(const tuple<float, float, float>& t) const {
        size_t h1 = hash<float>{}(get<0>(t));
        size_t h2 = hash<float>{}(get<1>(t));
        size_t h3 = hash<float>{}(get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct TupleEqual {
    bool operator()(const tuple<float, float, float>& t1, const tuple<float, float, float>& t2) const {
        return t1 == t2;
    }
};

class ZombiePathfinder {
public:
    ZombiePathfinder() = default;

    ZombiePathfinder(float startX, float startY, float startZ, float goalX, float goalY, float goalZ)
        : startX(startX), startY(startY), startZ(startZ), goalX(goalX), goalY(goalY), goalZ(goalZ) {}

    void Run(vector<tuple<float, float, float>>& t, int patroltype);

    void UpdatePathFinder(float startX, float startY, float startZ, float goalX, float goalY, float goalZ);
    float OBSTACLE_RADIUS = 25.f;
    unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash> EdgesMap;

private:
    float startX, startY, startZ;
    float goalX, goalY, goalZ;
    float beforegoalX = 0.f;
    float beforegoalY = 0.f;
    float beforegoalZ = 0.f;
    int  PatrolType = 0; // 0일때는 일반 동작 , 1일때는 patrol


    FLOOR floor;
    vector<tuple<float, float, float>> validPositions;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> positionSet;
    vector<tuple<float, float, float>> obstacles;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> obstacleSet;
    const float OBSTACLE_CHECK_INTERVAL = 25.0f;

    void SetObstacleRadius(float radious) { OBSTACLE_RADIUS = radious; }
    void SetPatrolType(int patroltype) { PatrolType = patroltype; }

    void DetermineFloor();

    bool LoadNewAStarPositions();

    bool LoadPositions();

    bool LoadObstacles();

    bool LoadFile(const string& filePath, vector<tuple<float, float, float>>& positions,
        unordered_set<tuple<float, float, float>, TupleHash, TupleEqual>& positionSet);

    //NewAstar
    bool LoadEdgesMap(const string& filePath, 
        vector<tuple<float, float, float>>& positions,
        unordered_map<tuple<float, float, float>,vector<pair<tuple<float, float, float>, float>>, TupleHash>& EdgesMap);



    void PrintPositions();

    void PrintObstacles();

    void FindPath(vector<tuple<float, float, float>>& t);


    vector<Node> FindNeighbors(const Node& current);

    vector<Node> AStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ,
        const vector<tuple<float, float, float>>& validPositions,
        const vector<tuple<float, float, float>>& obstacles); 
    
    
    vector<Node> NewAStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ,
        const vector<tuple<float, float, float>>& validPositions);

    bool IsPathBlockedByObstacle(const Node& startNode, const Node& endNode);

    float FloorZPos();

    double Heuristic(float x1, float y1, float x2, float y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    double NewMaxeuristic(float x1, float y1, float x2, float y2, float maxWeight) {
        return maxWeight * sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    bool IsInObstacleRange(float x, float y, float z);

    double EuclideanDistance(float x1, float y1, float x2, float y2);
    tuple<float, float, float> FindClosestValidPosition(float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions);
};



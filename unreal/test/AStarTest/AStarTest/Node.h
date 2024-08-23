#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Node {
public:
    float x, y, z;
    double gCost, hCost;

    Node() = default;

    Node(float x, float y, float z, double gCost = 0, double hCost = 0)
        : x(x), y(y), z(z), gCost(gCost), hCost(hCost) {}

    double fCost() const {
        return gCost + hCost;
    }

    bool operator<(const Node& other) const {
        return fCost() > other.fCost();  // 오름차순 우선순위 큐 사용을 위해 부등호 반대
    }

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    struct Hash {
        size_t operator()(const Node& node) const {
            return hash<float>()(node.x) ^ hash<float>()(node.y) ^ hash<float>()(node.z);
        }
    };
};

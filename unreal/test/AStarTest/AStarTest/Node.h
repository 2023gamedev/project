#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>

using namespace std;

class Node {
public:
    int x, y, z;
    double gCost, hCost;

    Node() = default;

    Node(int x, int y, int z, double gCost = 0, double hCost = 0)
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
            return hash<int>()(node.x) ^ hash<int>()(node.y) ^ hash<int>()(node.z);
        }
    };
};

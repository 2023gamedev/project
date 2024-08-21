#include <set>
#include "Node.h"




double heuristic(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

vector<pair<int, int>> findNeighbors(int x, int y) {
    // 4방향 이동 (상하좌우)
    return { {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1} };
}

bool isObstacle(int x, int y, const set<pair<int, int>>& obstacles) {
    return obstacles.find({ x, y }) != obstacles.end();
}

vector<Node> aStar(int startX, int startY, int startZ, int goalX, int goalY, int goalZ, const set<pair<int, int>>& obstacles) {
    priority_queue<Node> openSet;
    unordered_map<Node, Node, Node::Hash> cameFrom;
    unordered_map<Node, double, Node::Hash> gScore;

    Node start(startX, startY, startZ, 0, heuristic(startX, startY, goalX, goalY));
    openSet.push(start);
    gScore[start] = 0;

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == goalX && current.y == goalY && current.z == goalZ) {
            vector<Node> path;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (auto& neighborPos : findNeighbors(current.x, current.y)) {
            int neighborX = neighborPos.first;
            int neighborY = neighborPos.second;

            // 장애물 체크
            if (isObstacle(neighborX, neighborY, obstacles)) {
                continue; // 장애물이 있으면 이웃 노드를 무시
            }

            Node neighbor(neighborX, neighborY, current.z);

            double tentativeGScore = gScore[current] + 1;

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                double hCost = heuristic(neighborX, neighborY, goalX, goalY);
                openSet.push(Node(neighborX, neighborY, neighbor.z, tentativeGScore, hCost));
            }
        }
    }

    return {}; // 경로를 찾지 못함
}

int main() {
    int startX = 0, startY = 0, startZ = 0;
    int goalX = 5, goalY = 5, goalZ = 0;


    set<pair<int, int>> obstacles;
    obstacles.insert({ 2, 2 });
    obstacles.insert({ 3, 3 });
    obstacles.insert({ 4, 4 });

    vector<Node> path = aStar(startX, startY, startZ, goalX, goalY, goalZ, obstacles);


    if (!path.empty()) {
        cout << "Path found:\n";
        for (const auto& node : path) {
            cout << "(" << node.x << ", " << node.y << ", " << node.z << ")\n";
        }
    }
    else {
        cout << "No path found.\n";
    }

    return 0;
}
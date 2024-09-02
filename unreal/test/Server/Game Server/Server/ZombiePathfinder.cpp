#include <iomanip>

#include "ZombiePathfinder.h"


void ZombiePathfinder::Run(vector<tuple<float, float, float>>& t)
{
    DetermineFloor();
    if (LoadPositions()) {
        //PrintPositions();
        if (LoadObstacles()) {
            //PrintObstacles();
            FindPath(t);
        }
        else {
            cerr << "Failed to load obstacles." << endl;
        }
    }
}

void ZombiePathfinder::DetermineFloor()
{
    if (startZ < 800.f) {
        floor = FLOOR::FLOOR_B2;
    }
    else if (startZ < 1800.f) {
        floor = FLOOR::FLOOR_B1;
    }
    else if (startZ < 2500.f) {
        floor = FLOOR::FLOOR_F1;
    }
    else if (startZ < 3600.f) {
        floor = FLOOR::FLOOR_F2;
    }
    else {
        floor = FLOOR::FLOOR_F3;
    }
}

bool ZombiePathfinder::LoadPositions()
{
    string filePath;
    switch (floor) {
    case FLOOR::FLOOR_B2: filePath = "../../../../Project/B2.txt"; break;
    case FLOOR::FLOOR_B1: filePath = "../../../../Project/B1.txt"; break;
    case FLOOR::FLOOR_F1: filePath = "../../../../Project/F1.txt"; break;
    case FLOOR::FLOOR_F2: filePath = "../../../../Project/F2.txt"; break;
    case FLOOR::FLOOR_F3: filePath = "../../../../Project/F3.txt"; break;
    default: cout << "FILE ERROR!!!!" << endl; return false;
    }
    return LoadFile(filePath, validPositions, positionSet);
}

bool ZombiePathfinder::LoadObstacles()
{
    string filePathOb;
    switch (floor) {
    case FLOOR::FLOOR_B2: filePathOb = "../../../../Project/OB2.txt"; break;
    case FLOOR::FLOOR_B1: filePathOb = "../../../../Project/OB1.txt"; break;
    case FLOOR::FLOOR_F1: filePathOb = "../../../../Project/OF1.txt"; break;
    case FLOOR::FLOOR_F2: filePathOb = "../../../../Project/OF2.txt"; break;
    case FLOOR::FLOOR_F3: filePathOb = "../../../../Project/OF3.txt"; break;
    default: cout << "FILE ERROR!!!!" << endl; return false;
    }
    return LoadFile(filePathOb, obstacles, obstacleSet);
}

bool ZombiePathfinder::LoadFile(const string& filePath, vector<tuple<float, float, float>>& positions, unordered_set<tuple<float, float, float>, TupleHash, TupleEqual>& positionSet)
{
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filePath << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        float x, y, z;
        char comma;
        if (ss >> x >> comma >> y >> comma >> z) {
            tuple<float, float, float> position = make_tuple(x, y, z);
            if (positionSet.find(position) == positionSet.end()) {
                positionSet.emplace(position);
                positions.emplace_back(position);
            }
        }
        else {
            cerr << "Invalid position: " << line << endl;
        }
    }
    file.close();
    return true;
}

void ZombiePathfinder::PrintPositions()
{
    cout << "Valid Positions:" << endl;
    for (const auto& pos : validPositions) {
        cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
    }
}

void ZombiePathfinder::PrintObstacles()
{
    cout << "Obstacles Positions:" << endl;
    for (const auto& pos : obstacles) {
        cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
    }
}

void ZombiePathfinder::FindPath(vector<tuple<float, float, float>>& t)
{
    vector<Node> path = AStar(startX, startY, startZ, goalX, goalY, goalZ, validPositions, obstacles);
    if (!path.empty()) {
        cout << "Path found:\n";
        for (const auto& node : path) {
            cout << "( " << std::setw(8) << node.x << ", " << std::setw(8) << node.y << ", " << std::setw(8) << node.z << " )\n";

            t.emplace_back(make_tuple(node.x, node.y, node.z));
        }
    }
    else {
        cout << "No path found.\n";
    }
}

vector<Node> ZombiePathfinder::FindNeighbors(const Node& current)
{
    vector<Node> neighbors;
    for (const auto& pos : validPositions) {
        float nx = get<0>(pos);
        float ny = get<1>(pos);
        float nz = get<2>(pos);
        if (nz >= current.z - 400.f && nz <= current.z + 400.f && !(nx == current.x && ny == current.y)) {
            double dist = Heuristic(current.x, current.y, nx, ny);
            if (dist <= 800.f && !IsInObstacleRange(nx, ny, nz)) {
                double hCost = Heuristic(nx, ny, goalX, goalY);
                neighbors.push_back(Node(nx, ny, nz, current.gCost + dist, hCost));
            }
        }
    }
    return neighbors;
}

double ZombiePathfinder::EuclideanDistance(float x1, float y1,float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// 함수: 주어진 좌표들 내에서 목표 좌표와 가장 가까운 좌표 찾기
tuple<float, float, float> ZombiePathfinder::FindClosestValidPosition(float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions) {
    tuple<float, float, float> closestPosition = validPositions[0];
    double minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));

    for (const auto& position : validPositions) {
        double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));
        if (distance < minDistance) {
            closestPosition = position;
            minDistance = distance;
        }
    }
    return closestPosition;
}
vector<Node> ZombiePathfinder::AStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions, const vector<tuple<float, float, float>>& obstacles)
{
    float SimilargoalX;
    float SimilargoalY;
    float SimilargoalZ;
    tie(SimilargoalX, SimilargoalY, SimilargoalZ) = FindClosestValidPosition(goalX, goalY, goalZ, validPositions);
    SimilargoalZ = goalZ;

    priority_queue<Node> openSet;
    unordered_map<Node, Node, Node::Hash> cameFrom;
    unordered_map<Node, double, Node::Hash> gScore;

    Node start(startX, startY, startZ, 0, Heuristic(startX, startY, goalX, goalY));
    openSet.push(start);
    gScore[start] = 0;

    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        if (current.x == SimilargoalX && current.y == SimilargoalY) {
            vector<Node> path;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());

            if (SimilargoalX != goalX || SimilargoalY != goalY) {
                path.push_back(Node(goalX, goalY, goalZ, 0, 0));
            }

            return path; // 경로를 성공적으로 찾음
        }

        for (Node neighbor : FindNeighbors(current)) {
            if (IsPathBlockedByObstacle(current, neighbor)) {
                continue; // 장애물이 있는 경로는 무시하고 다른 경로를 탐색
            }

            double tentativeGScore = gScore[current] + Heuristic(current.x, current.y, neighbor.x, neighbor.y);

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
            }
        }
    }
    return {}; // 가능한 경로를 찾지 못함
}

bool ZombiePathfinder::IsPathBlockedByObstacle(const Node& startNode, const Node& endNode) {
    float dx = endNode.x - startNode.x;
    float dy = endNode.y - startNode.y;
    float dz = endNode.z - startNode.z;

    float length = sqrt(dx * dx + dy * dy + dz * dz);
    int numSteps = static_cast<int>(length / OBSTACLE_CHECK_INTERVAL); // 장애물 체크 간격에 따라 조정

    for (int i = 0; i <= numSteps; ++i) {
        float t = static_cast<float>(i) / numSteps;
        float x = startNode.x + t * dx;
        float y = startNode.y + t * dy;
        float z = startNode.z + t * dz;

        if (IsInObstacleRange(x, y, z)) {
            return true; // 경로 상에 장애물이 있음
        }
    }

    return false; // 경로 상에 장애물이 없음
}
bool ZombiePathfinder::IsInObstacleRange(float x, float y, float z)
{
    for (const auto& obs : obstacles) {
        float ox = get<0>(obs);
        float oy = get<1>(obs);

        float dx = x - ox;
        float dy = y - oy;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= OBSTACLE_RADIUS) { // 장애물 부분 수정 필요
            return true;
        }
    }
    return false;
}

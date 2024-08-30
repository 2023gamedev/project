#include "ZombiePathfinder.h"



void ZombiePathfinder::Run(vector<tuple<float, float, float>>& t)
{
    DetermineFloor();
    if (LoadPositions()) {
        if (LoadObstacles()) {
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
    string filePathOb = "../../../../Project/ObstacleNodes.txt";
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
            cout << "(" << node.x << ", " << node.y << ", " << node.z << ")\n";

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
        if (nz == current.z && !(nx == current.x && ny == current.y)) {
            double dist = Heuristic(current.x, current.y, nx, ny);
            if (dist <= 800.f && !IsInObstacleRange(nx, ny, nz)) {
                double hCost = Heuristic(nx, ny, goalX, goalY);
                neighbors.push_back(Node(nx, ny, nz, current.gCost + dist, hCost));
            }
        }
    }
    return neighbors;
}

vector<Node> ZombiePathfinder::AStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions, const vector<tuple<float, float, float>>& obstacles)
{
    priority_queue<Node> openSet;
    unordered_map<Node, Node, Node::Hash> cameFrom;
    unordered_map<Node, double, Node::Hash> gScore;

    Node start(startX, startY, startZ, 0, Heuristic(startX, startY, goalX, goalY));
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

        for (Node neighbor : FindNeighbors(current)) {
            double tentativeGScore = gScore[current] + Heuristic(current.x, current.y, neighbor.x, neighbor.y);

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
            }
        }
    }
    return {};
}

bool ZombiePathfinder::IsInObstacleRange(float x, float y, float z)
{
    for (const auto& obs : obstacles) {
        float ox = get<0>(obs);
        float oy = get<1>(obs);

        float dx = x - ox;
        float dy = y - oy;
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= OBSTACLE_RADIUS) {
            return true;
        }
    }
    return false;
}

#include <iomanip>

#include "ZombiePathfinder.h"


void ZombiePathfinder::Run(vector<tuple<float, float, float>>& t, int patroltype)
{
    SetPatrolType(patroltype);
    DetermineFloor();
    if (LoadPositions()/* LoadNewAStarPositions()*/) {
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

//NewAstar
bool ZombiePathfinder::LoadNewAStarPositions()
{
    string filePath;

    // ������ ���� ��� ����
    switch (floor) {
    case FLOOR::FLOOR_B2: filePath = "../../../../Project/EdgesB2.txt"; break;
    case FLOOR::FLOOR_B1: filePath = "../../../../Project/EdgesB1.txt"; break;
    case FLOOR::FLOOR_F1: filePath = "../../../../Project/EdgesF1.txt"; break;
    case FLOOR::FLOOR_F2: filePath = "../../../../Project/EdgesF2.txt"; break;
    case FLOOR::FLOOR_F3: filePath = "../../../../Project/EdgesF3.txt"; break;
    default:
        cout << "FILE ERROR!!!!" << endl;
        return false;
    }

    // EdgesMap�� �ҷ����� �������� ����
    return LoadEdgesMap(filePath, validPositions,EdgesMap);
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

bool ZombiePathfinder::LoadEdgesMap(const string& filePath, vector<tuple<float, float, float>>& positions, unordered_map<tuple<float, float, float>, vector<pair<tuple<float, float, float>, float>>, TupleHash>& EdgesMap)
{
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Cannot open file: " << filePath << endl;
        return false;
    }

    string line;
    tuple<float, float, float> currentNode;
    while (getline(file, line)) {
        stringstream ss(line);

        if (line.find("Node:") != string::npos) {
            // Node ���� �Ľ�
            float x, y, z;
            char comma;
            ss.ignore(5); // "Node: " ����
            if (ss >> x >> comma >> y >> comma >> z) {
                currentNode = make_tuple(x, y, z);
                EdgesMap[currentNode] = {};  // ���ο� ��� �߰�

                // validPositions�� currentNode �߰�
                positions.push_back(currentNode);
            }
        }
        else if (line.find("Neighbor:") != string::npos) {
            // Neighbor ���� �Ľ�
            float nx, ny, nz, weight;
            char comma;
            ss.ignore(9); // "Neighbor: " ����
            if (ss >> nx >> comma >> ny >> comma >> nz >> comma >> weight) {
                tuple<float, float, float> neighbor = make_tuple(nx, ny, nz);
                EdgesMap[currentNode].emplace_back(neighbor, weight);  // �̿� ��� �� ����ġ �߰�
            }
        }
    }

    file.close();
    return true;
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
    // cout << beforegoalX << " , " << beforegoalY << " , " << beforegoalZ << " , " << endl;
    if (beforegoalX == goalX && beforegoalY == goalY && beforegoalZ == goalZ) {
        return;
    }

    vector<Node> path = AStar(startX, startY, startZ, goalX, goalY, goalZ, validPositions, obstacles);
    //vector<Node> path = NewAStar(startX, startY, startZ, goalX, goalY, goalZ, validPositions);
    if (!path.empty()) {
        t.clear();
        //cout << "Path found:\n";
        for (auto it = path.begin(); it != path.end(); ++it) {
            const auto& node = *it;


            if (it != path.end() - 1) {
                //cout << "( " << std::setw(8) << node.x << ", " << std::setw(8) << node.y << ", " << std::setw(8) << node.z << " )";
            }
            else {
                if (PatrolType == 0) {
                    //cout << "( " << std::setw(8) << node.x << ", " << std::setw(8) << node.y << ", " << std::setw(8) << node.z << " )";
                }
            }

            //cout << "\n";
            t.emplace_back(make_tuple(node.x, node.y, node.z));
        }
    }
    else {
        //cout << "No path found.\n";
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

void ZombiePathfinder::UpdatePathFinder(float startx, float starty, float startz, float goalx, float goaly, float goalz)
{
    startX = startx;
    startY = starty;
    startZ = startz;
    goalX = goalx;
    goalY = goaly;
    goalZ= goalz;
}

// �Լ�: �־��� ��ǥ�� ������ ��ǥ ��ǥ�� ���� ����� ��ǥ ã��
tuple<float, float, float> ZombiePathfinder::FindClosestValidPosition(float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions) {
    tuple<float, float, float> closestPosition = validPositions[0];
    double minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));

    for (const auto& position : validPositions) {
        double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));

        if (distance == 0) {
            return position;
        }

        if (distance < minDistance) {
            closestPosition = position;
            minDistance = distance;
        }
    }
    return closestPosition;
}


//New AStar

vector<Node> ZombiePathfinder::NewAStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions)
{
    // ���� ������ ��ǥ ������ ������ �ٷ� ��ȯ
    if (startX == goalX && startY == goalY && startZ == goalZ) {
        return { Node(startX, startY, startZ, 0, 0) };
    }

    // ��ǥ ������ ���� ����� ��ȿ�� ���� ã��

    float SimilarStartX, SimilarStartY, SimilarStartZ;
    tie(SimilarStartX, SimilarStartY, SimilarStartZ) = FindClosestValidPosition(startX, startY, startZ, validPositions);
    SimilarStartZ = startZ;

    float SimilargoalX, SimilargoalY, SimilargoalZ;
    tie(SimilargoalX, SimilargoalY, SimilargoalZ) = FindClosestValidPosition(goalX, goalY, goalZ, validPositions);
    SimilargoalZ = goalZ;

    // A* �˰����� ���� ������ ����
    priority_queue<Node> openSet;
    unordered_map<Node, Node, Node::Hash> cameFrom;
    unordered_map<Node, double, Node::Hash> gScore;

    // ���� ��� �ʱ�ȭ
    Node Realstart(startX, startY, startZ, 0, Heuristic(startX, startY, SimilargoalX, SimilargoalY));
    //openSet.push(Realstart);


    Node start(SimilarStartX, SimilarStartY, SimilarStartZ, 0, Heuristic(SimilarStartX, SimilarStartY, SimilargoalX, SimilargoalY));
    if (startX != SimilarStartX || startY != SimilarStartY) {
        
        openSet.push(start);
        gScore[start] = 0;

    }
    else
    {
        openSet.push(Realstart);
        gScore[Realstart] = 0;
    }

    

    // A* Ž�� ����
    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        // ��ǥ ������ �����ϸ� ��� ��ȯ
        if (current.x == SimilargoalX && current.y == SimilargoalY) {
            vector<Node> path;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            if (startX != SimilarStartX || startY != SimilarStartY) {
                path.push_back(start);
            }
            else
            {
                path.push_back(Realstart);
            }

            reverse(path.begin(), path.end());

            // ���� ��ǥ ���� Ȯ�� �� �߰�
            if (path.back().x != goalX || path.back().y != goalY) {
                path.push_back(Node(goalX, goalY, goalZ, 0, 0));
            }

            return path;
        }

        // ���� ����� �̿� Ž�� (EdgesMap ���)
        tuple<float, float, float> currentPos = make_tuple(current.x, current.y, current.z);

        // EdgesMap�� ���� ����� �̿��� ������ �ִ��� Ȯ��
        if (EdgesMap.find(currentPos) != EdgesMap.end()) {
            const vector<pair<tuple<float, float, float>, float>>& neighbors = EdgesMap[currentPos];

            // �̿� ���鿡 ���� �ݺ�
            for (const auto& neighborData : neighbors) {
                tuple<float, float, float> neighborPos = neighborData.first;
                float edgeWeight = neighborData.second;  // Edge ����ġ

                Node neighbor(get<0>(neighborPos), get<1>(neighborPos), get<2>(neighborPos), 0, 0);


                // gScore ��� (������ ����ġ�� ���)
                double tentativeGScore = gScore[current] + edgeWeight;

                // �� ���� ��θ� �߰��ϸ� ������Ʈ
                if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentativeGScore;
                    double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                    openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
                }
            }
        }
    }

    // ��θ� ã�� ���ϸ� �� ��� ��ȯ
    return {};
}


vector<Node> ZombiePathfinder::AStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ, const vector<tuple<float, float, float>>& validPositions, const vector<tuple<float, float, float>>& obstacles)
{
    // ���� ������ ��ǥ ������ ���ٸ�
    if (startX == goalX && startY == goalY && startZ == goalZ) {
        return { Node(startX, startY, startZ, 0, 0) }; // ���� ������ ���Ե� ��� ��ȯ
    }

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

            //if (PatrolType == 0) { // �̷��� �� �� �ƿ� ���������� ����
            //    // ������ ��ο� ��ǥ ������ ������ Ȯ��
            //    if (path.back().x != goalX || path.back().y != goalY) {
            //        path.push_back(Node(goalX, goalY, goalZ, 0, 0));
            //    }
            //}

            // ������ ��ο� ��ǥ ������ ������ Ȯ��
            if (path.back().x != goalX || path.back().y != goalY) { 
                path.push_back(Node(goalX, goalY, goalZ, 0, 0));
            }

            beforegoalX = goalX;
            beforegoalY = goalY;
            beforegoalZ = goalZ;


            return path; // ��θ� ���������� ã��
        }

        for (Node neighbor : FindNeighbors(current)) {
            if (IsPathBlockedByObstacle(current, neighbor)) {
                continue; // ��ֹ��� �ִ� ��δ� �����ϰ� �ٸ� ��θ� Ž��
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
    return {}; // ������ ��θ� ã�� ����
}

bool ZombiePathfinder::IsPathBlockedByObstacle(const Node& startNode, const Node& endNode) {
    float dx = endNode.x - startNode.x;
    float dy = endNode.y - startNode.y;
    float dz = endNode.z - startNode.z;

    float length = sqrt(dx * dx + dy * dy);
    int numSteps = static_cast<int>(length / OBSTACLE_CHECK_INTERVAL); // ��ֹ� üũ ���ݿ� ���� ����

    for (int i = 0; i <= numSteps; ++i) {
        float t = static_cast<float>(i) / numSteps;
        float x = startNode.x + t * dx;
        float y = startNode.y + t * dy;
        float z = startNode.z + t * dz; // ��� z���� �ʿ������� ����

        if (IsInObstacleRange(x, y, z)) {
            return true; // ��� �� ��ֹ��� ����
        }
    }

    return false; // ��� �� ��ֹ��� ����
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

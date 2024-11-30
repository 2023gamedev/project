#include <iomanip>

#include "ZombiePathfinder.h"

#include "iocpServerClass.h"

void ZombiePathfinder::Run(vector<tuple<float, float, float>>& t, int patroltype)
{
    SetPatrolType(patroltype);
    //DetermineFloor();


    FindPath(t);

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

    //vector<Node> path = AStar(startX, startY, startZ, goalX, goalY, goalZ, validPositions, obstacles);
    vector<Node> path = NewAStar(startX, startY, startZ, goalX, goalY, goalZ);
    if (!path.empty()) {
        t.clear();
        //cout << "Path found:\n";
        for (auto it = path.begin(); it != path.end(); ++it) {
            const auto& node = *it;


            if (it != path.end() - 1) {
                //cout << "( " << std::setw(8) << node.x << ", " << std::setw(8) << node.y << ", " << std::setw(8) << node.z << " )" << endl;
            }
            else {
                if (PatrolType == 0) {
                    //cout << "( " << std::setw(8) << node.x << ", " << std::setw(8) << node.y << ", " << std::setw(8) << node.z << " )" << endl;
                }
            }

            //cout << "\n";
            t.emplace_back(make_tuple(node.x, node.y, node.z));
        }

        if (PatrolType == 0) {
            //cout << "PathUpdated!" << endl;
        }
        else if(PatrolType == 1) {
            //cout << "PathUpdated! -> RandPatrol" << endl;
        }
    }
    else {
        //cout << "[Error] No path found.\n";
    }
}

double ZombiePathfinder::EuclideanDistance(float x1, float y1,float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void ZombiePathfinder::UpdateStartGoal(float startx, float starty, float startz, float goalx, float goaly, float goalz)
{
    startX = startx;
    startY = starty;
    startZ = startz;
    goalX = goalx;
    goalY = goaly;
    goalZ= goalz;
}

// �Լ�: �־��� ��ǥ�� ������ ��ǥ ��ǥ�� ���� ����� ��ǥ ã��
tuple<float, float, float> ZombiePathfinder::FindClosestValidPosition(float goalX, float goalY, float goalZ) {

    tuple<float, float, float> closestPosition;
    double minDistance;
    if (floor == FLOOR::FLOOR_B2) {
        closestPosition = g_valispositionsB2[0];
        minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));
        for (const auto& position : g_valispositionsB2) {
            double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));

            if (distance == 0) {
                return position;
            }

            if (distance < minDistance) {
                closestPosition = position;
                minDistance = distance;
            }
        }
    }
    else if (floor == FLOOR::FLOOR_B1) {
        closestPosition = g_valispositionsB1[0];
        minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));
        for (const auto& position : g_valispositionsB1) {
            double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));

            if (distance == 0) {
                return position;
            }

            if (distance < minDistance) {
                closestPosition = position;
                minDistance = distance;
            }
        }
    }
    else if (floor == FLOOR::FLOOR_F1) {
        closestPosition = g_valispositionsF1[0];
        minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));
        for (const auto& position : g_valispositionsF1) {
            double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));

            if (distance == 0) {
                return position;
            }

            if (distance < minDistance) {
                closestPosition = position;
                minDistance = distance;
            }
        }
    }
    else if (floor == FLOOR::FLOOR_F2) {
        closestPosition = g_valispositionsF2[0];
        minDistance = EuclideanDistance(goalX, goalY, get<0>(closestPosition), get<1>(closestPosition));
        for (const auto& position : g_valispositionsF2) {
            double distance = EuclideanDistance(goalX, goalY, get<0>(position), get<1>(position));

            if (distance == 0) {
                return position;
            }

            if (distance < minDistance) {
                closestPosition = position;
                minDistance = distance;
            }
        }
    }

    return closestPosition;
}

float ZombiePathfinder::FloorZPos()
{
    float currentz;
    switch (floor) {

    case FLOOR::FLOOR_B2: currentz = 101.f; break;
    case FLOOR::FLOOR_B1: currentz = 1051.f; break;
    case FLOOR::FLOOR_F1: currentz = 2001.f; break;
    case FLOOR::FLOOR_F2: currentz = 2962.f; break;
    case FLOOR::FLOOR_F3: currentz = 0.f; break;

    }

    return currentz;
}

//New AStar

vector<Node> ZombiePathfinder::NewAStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ)
{

    //cout << "NewAStarSTART!!" << endl;
    // ���� ������ ��ǥ ������ ������ �ٷ� ��ȯ
    if (startX == goalX && startY == goalY && startZ == goalZ) {
        //cout << "EQUAL PATH" << endl;
        return { Node(startX, startY, startZ, 0, 0) };
    }

    
    // ��ǥ ������ ���� ����� ��ȿ�� ���� ã��

    float SimilarStartX, SimilarStartY, SimilarStartZ;
    tie(SimilarStartX, SimilarStartY, SimilarStartZ) = FindClosestValidPosition(startX, startY, startZ);
    SimilarStartZ = startZ;

    float SimilargoalX, SimilargoalY, SimilargoalZ;
    tie(SimilargoalX, SimilargoalY, SimilargoalZ) = FindClosestValidPosition(goalX, goalY, goalZ);
    SimilargoalZ = goalZ;

    // A* �˰����� ���� ������ ����
    priority_queue<Node> openSet;
    unordered_map<Node, Node, Node::Hash> cameFrom;
    unordered_map<Node, double, Node::Hash> gScore;

    // ���� ��� �ʱ�ȭ
    Node Realstart(startX, startY, startZ, 0, NewMaxeuristic(startX, startY, SimilargoalX, SimilargoalY, sqrt(2)));

    Node start(SimilarStartX, SimilarStartY, SimilarStartZ, 0, NewMaxeuristic(SimilarStartX, SimilarStartY, SimilargoalX, SimilargoalY, sqrt(2)));
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
    int i = 0; // ������(�󸶳� �ɸ���)
    while (!openSet.empty()) {
        if (i == 0) {
            i++;
        }
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

            beforegoalX = goalX;
            beforegoalY = goalY;
            beforegoalZ = goalZ;

            //cout << "Good Path" << endl;
            return path;
        }

        tuple<float, float, float> currentPos = make_tuple(current.x, current.y, FloorZPos());

        if (floor == FLOOR::FLOOR_B2) {
            // EdgesMap�� ���� ����� �̿��� ������ �ִ��� Ȯ��
            if (g_EdgesMapB2.find(currentPos) != g_EdgesMapB2.end()) {
                const vector<pair<tuple<float, float, float>, float>>& neighbors = g_EdgesMapB2[currentPos];

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
                        //double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                        double hCost = NewMaxeuristic(neighbor.x, neighbor.y, goalX, goalY, sqrt(2));
                        openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
                    }
                }
            }
        }
        else if (floor == FLOOR::FLOOR_B1) {
            // EdgesMap�� ���� ����� �̿��� ������ �ִ��� Ȯ��
            if (g_EdgesMapB1.find(currentPos) != g_EdgesMapB1.end()) {
                const vector<pair<tuple<float, float, float>, float>>& neighbors = g_EdgesMapB1[currentPos];

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
                        //double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                        double hCost = NewMaxeuristic(neighbor.x, neighbor.y, goalX, goalY, sqrt(2));
                        openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
                    }
                }
            }
        }
        else if (floor == FLOOR::FLOOR_F1) {
            // EdgesMap�� ���� ����� �̿��� ������ �ִ��� Ȯ��
            if (g_EdgesMapF1.find(currentPos) != g_EdgesMapF1.end()) {
                const vector<pair<tuple<float, float, float>, float>>& neighbors = g_EdgesMapF1[currentPos];

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
                        //double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                        double hCost = NewMaxeuristic(neighbor.x, neighbor.y, goalX, goalY, sqrt(2));
                        openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
                    }
                }
            }
        }
        else if (floor == FLOOR::FLOOR_F2) {
            // EdgesMap�� ���� ����� �̿��� ������ �ִ��� Ȯ��
            if (g_EdgesMapF2.find(currentPos) != g_EdgesMapF2.end()) {
                const vector<pair<tuple<float, float, float>, float>>& neighbors = g_EdgesMapF2[currentPos];

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
                        //double hCost = Heuristic(neighbor.x, neighbor.y, goalX, goalY);
                        double hCost = NewMaxeuristic(neighbor.x, neighbor.y, goalX, goalY, sqrt(2));
                        openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
                    }
                }
            }
        }

        

    }

    // ��θ� ã�� ���ϸ� �� ��� ��ȯ
    return {};
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

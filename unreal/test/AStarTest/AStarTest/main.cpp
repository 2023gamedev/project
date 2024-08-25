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


double heuristic(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
//
//bool isObstacle(float x, float y, float z, const vector<tuple<float, float, float>>& obstacles) {
//    return find(obstacles.begin(), obstacles.end(), make_tuple(x, y, z)) != obstacles.end();
//}

// 일정 거리 이내의 장애물로 간주하는 거리
const float OBSTACLE_RADIUS = 200.0f;

bool isInObstacleRange(float x, float y, float z, const vector<tuple<float, float, float>>& obstacles) {
    for (const auto& obs : obstacles) {
        float ox = get<0>(obs);
        float oy = get<1>(obs);
        float oz = get<2>(obs);

        float dx = x - ox;
        float dy = y - oy;
        float dz = z - oz;
        float distance = std::sqrt(dx * dx + dy * dy + dz * dz);

        // 거리가 장애물 반경 이하이면 충돌
        if (distance <= OBSTACLE_RADIUS) {
            return true;
        }
    }
    return false;
}


vector<Node> findNeighbors(const Node& current, const vector<tuple<float, float, float>>& validPositions, float goalX, float goalY, const vector<tuple<float, float, float>>& obstacles) {
    vector<Node> neighbors;

    for (const auto& pos : validPositions) {
        float nx = get<0>(pos);
        float ny = get<1>(pos);
        float nz = get<2>(pos);

        // z 값이 동일하고 현재 위치와 다른 노드만 고려
        if (nz == current.z && !(nx == current.x && ny == current.y)) {
            // 현재 노드와의 거리 계산
            double dist = heuristic(current.x, current.y, nx, ny);
            if (dist <= 800.f) { // 일정 거리 이내에 있는 노드만 이웃으로 고려
                if (isInObstacleRange(nx, ny, nz, obstacles)) {
                    continue; 
                }
                double hCost = heuristic(nx, ny, goalX, goalY);
                neighbors.push_back(Node(nx, ny, nz, current.gCost + dist, hCost));
            }
        }
    }

    return neighbors;
}

vector<Node> aStar(float startX, float startY, float startZ, float goalX, float goalY, float goalZ,
    const vector<tuple<float, float, float>>& validPositions, const vector<tuple<float, float, float>>& obstacles) {

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

        for (Node neighbor : findNeighbors(current, validPositions, goalX, goalY, obstacles)) {
            double tentativeGScore = gScore[current] + heuristic(current.x, current.y, neighbor.x, neighbor.y);

            if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                double hCost = heuristic(neighbor.x, neighbor.y, goalX, goalY);
                openSet.push(Node(neighbor.x, neighbor.y, neighbor.z, tentativeGScore, hCost));
            }
        }
    }

    return {};
}

// load Position txt - b1,b2,f1,f2,f3 파일 읽는곳
bool loadPositions(FLOOR floor, const string& filePath,
    vector<tuple<float, float, float>>& validPositions,
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual>& positionSet) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filePath << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        float x, y, z;
        char comma;

        if (ss >> x >> comma >> y >> comma >> z) {
            tuple<float, float, float> position = make_tuple(x, y, z);

            // 중복 확인 후 vector에 추가
            if (positionSet.find(position) == positionSet.end()) {
                positionSet.emplace(position);
                validPositions.emplace_back(position);
            }
        }
        else {
            cerr << "Not valid position: " << line << endl;
        }
    }

    file.close();
    return true;
}

// load Obstacles txt - obstacles 파일 읽는곳 둘이 구조는 같지만 함수 분리
bool loadObstacles(const string& filePath,
    vector<tuple<float, float, float>>& obstacles,
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual>& obstacleSet) {
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filePath << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        float x, y, z;
        char comma;

        if (ss >> x >> comma >> y >> comma >> z) {
            tuple<float, float, float> obstacle = make_tuple(x, y, z);

            // 중복 확인 후 vector에 추가
            if (obstacleSet.find(obstacle) == obstacleSet.end()) {
                obstacleSet.emplace(obstacle);
                obstacles.emplace_back(obstacle);
            }
        }
        else {
            cerr << "Not valid obstacle position: " << line << endl;
        }
    }

    file.close();
    return true;
}

int main()
{
    float startX = 988.0f, startY = 2964.0f, startZ = 952.0f;
    
    //Test1
    //float goalX = 304.0f, goalY = 3629.0f, goalZ = 952.0f;
    /*(988, 2964, 952)
        (912, 2964, 952)
        (494, 3629, 952)
        (304, 3629, 952)*/

    //Test2
    //float goalX = 133.0f, goalY = 3952.0f, goalZ = 952.0f;

    // 장애물 없을 시
    /* 
    (988, 2964, 952)
        (912, 2964, 952)
        (494, 3629, 952)
        (133, 3952, 952)*/

    // 장애물 있을 시 
    //(988, 2964, 952)
    //    (893, 3648, 952)
    //    (456, 3952, 952)
    //    (133, 3952, 952)


    //Test3
    float goalX = 2299.0f, goalY = 3857.0f, goalZ = 952.0f;
    
    // 장애물 없을 시
    //(988, 2964, 952)
    //    (1102, 2964, 952)
    //    (1501, 3629, 952)
    //    (1976, 3762, 952)
    //    (2299, 3857, 952)

    // 장애물 있을 시 
    //    (988, 2964, 952)
    //    (1292, 3648, 952)
    //    (1976, 3762, 952)
    //    (2299, 3857, 952)

    FLOOR floor;


    // 파일 경로 설정
    
    string filePathB2 = "../../../Project/B2.txt";
    string filePathB1 = "../../../Project/B1.txt";
    string filePathF1 = "../../../Project/F1.txt";
    string filePathF2 = "../../../Project/F2.txt";
    string filePathF3 = "../../../Project/F3.txt";



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

    vector<tuple<float, float, float>> validPositions;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> positionSet;

    bool success = false;
    switch (floor) {
    case FLOOR::FLOOR_B2:
        success = loadPositions(floor, filePathB2, validPositions, positionSet);
        break;
    case FLOOR::FLOOR_B1:
        success = loadPositions(floor, filePathB1, validPositions, positionSet);
        break;
    case FLOOR::FLOOR_F1:
        success = loadPositions(floor, filePathF1, validPositions, positionSet);
        break;
    case FLOOR::FLOOR_F2:
        success = loadPositions(floor, filePathF2, validPositions, positionSet);
        break;
    case FLOOR::FLOOR_F3:
        success = loadPositions(floor, filePathF3, validPositions, positionSet);
        break;
    default:
        cout << "FILE ERROR!!!!" << endl;
        return 1;
    }

    if (success) {
        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }
    }

    cout << "==========================================================================================" << endl;

    string filePathOb = "../../../Project/ObstacleNodes.txt";

    vector<tuple<float, float, float>> obstacles;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> obstacleSet;



    if (!loadObstacles(filePathOb, obstacles, obstacleSet)) {
        cerr << "Failed to load obstacles." << endl;
        return 1;
    }

    if (success) {
        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }

        cout << "Obstacles Positions:" << endl;
        for (const auto& pos : obstacles) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }
    }


    vector<Node> path = aStar(startX, startY, startZ, goalX, goalY, goalZ, validPositions, obstacles);

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

// Z값 각 층마다 txt파일로 출력해놓기
// 2가지의 경로(네비메쉬, 라인트레이스) 사용해서 네비메쉬는 경로 라인트레이스는 장애물로 사용하기
// 좀비 시작위치, 플레이어 위치(도착), 네비메쉬 위치들, 장애물 위치 범위들에 따른 함수 수정 필요


// 문제점
// 파일에 내용을 제대로 안받아오는 중

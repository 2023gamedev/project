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

bool isObstacle(float x, float y, float z, const vector<tuple<float, float, float>>& obstacles) {
    return find(obstacles.begin(), obstacles.end(), make_tuple(x, y, z)) != obstacles.end();
}

vector<Node> findNeighbors(const Node& current, const vector<tuple<float, float, float>>& validPositions, float goalX, float goalY) {
    vector<Node> neighbors;
    Node closestNeighbor;
    double closestDist = numeric_limits<double>::infinity();

    for (const auto& pos : validPositions) {
        float nx = get<0>(pos);
        float ny = get<1>(pos);
        float nz = get<2>(pos);

        // z 값이 동일하고 현재 위치와 다를 경우만 고려
        if (nz == current.z && !(nx == current.x && ny == current.y)) {
            double distance = heuristic(nx, ny, goalX, goalY);
            if (distance < closestDist) {
                closestDist = distance;
                closestNeighbor = Node(nx, ny, nz, current.gCost + heuristic(current.x, current.y, nx, ny), distance);
            }
        }
    }

    if (closestDist < numeric_limits<double>::infinity()) {
        neighbors.push_back(closestNeighbor);
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

        for (Node neighbor : findNeighbors(current, validPositions, goalX, goalY)) {
            if (isObstacle(neighbor.x, neighbor.y, neighbor.z, obstacles)) {
                continue;
            }

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

int main()
{
    float startX = 988.0f, startY = 2964.0f, startZ = 952.0f;
    float goalX = 304.0f, goalY = 3629.0f, goalZ = 952.0f;

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
    string line;

    if (floor == FLOOR::FLOOR_B2) {
        ifstream fileB2(filePathB2);

        if (!fileB2.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePathB2 << endl;
            return 1;
        }


        while (getline(fileB2, line)) {
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

        fileB2.close();

        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }

    }
    else if(floor == FLOOR::FLOOR_B1) {
        ifstream fileB1(filePathB1);

        if (!fileB1.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePathB1 << endl;
            return 1;
        }


        while (getline(fileB1, line)) {
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

        fileB1.close();

        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }
    }
    else if (floor == FLOOR::FLOOR_F1) {
        ifstream fileF1(filePathF1);

        if (!fileF1.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePathF1 << endl;
            return 1;
        }


        while (getline(fileF1, line)) {
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

        fileF1.close();

        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }
    }
    else if (floor == FLOOR::FLOOR_F2) {
        ifstream fileF2(filePathF2);

        if (!fileF2.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePathF2 << endl;
            return 1;
        }


        while (getline(fileF2, line)) {
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

        fileF2.close();

        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }

    }
    else if (floor == FLOOR::FLOOR_F3) {
        ifstream fileF3(filePathF3);

        if (!fileF3.is_open()) {
            cerr << "파일을 열 수 없습니다: " << filePathF3 << endl;
            return 1;
        }


        while (getline(fileF3, line)) {
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

        fileF3.close();

        cout << "Valid Positions:" << endl;
        for (const auto& pos : validPositions) {
            cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
        }

    }
    else {
        cout << "FILE ERROR!!!!" << endl;
        return 0;
    }



    cout << "==========================================================================================" << endl;

    string filePathOb = "../../../Project/ObstacleNodes.txt";
    ifstream fileOb(filePathOb);

    if (!fileOb.is_open()) {
        cerr << "파일을 열 수 없습니다.: " << filePathOb << endl;
        return 1;
    }

    vector<tuple<float, float, float>> obstacles;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> obstacleSet;
    string lineOb;

    while (getline(fileOb, lineOb)) {
        stringstream ss(lineOb);
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
            cerr << "Not valid obstacle position: " << lineOb << endl;
        }
    }

    fileOb.close();

    cout << "Obstacles Positions:" << endl;
    for (const auto& pos : obstacles) {
        cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
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

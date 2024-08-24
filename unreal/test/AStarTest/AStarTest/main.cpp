#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include "Node.h"


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

        // z ���� �����ϰ� ���� ��ġ�� �ٸ� ��츸 ���
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
    float startX = 0.0f, startY = 0.0f, startZ = 0.0f;
    float goalX = 5.0f, goalY = 5.0f, goalZ = 0.0f;

    // ���� ��� ����
    string filePath = "../../../Project/B1.txt";
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filePath << endl;
        return 1;
    }

    vector<tuple<float, float, float>> validPositions;
    unordered_set<tuple<float, float, float>, TupleHash, TupleEqual> positionSet;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        float x, y, z;
        char comma;

        if (ss >> x >> comma >> y >> comma >> z) {
            tuple<float, float, float> position = make_tuple(x, y, z);

            // �ߺ� Ȯ�� �� vector�� �߰�
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

    cout << "Valid Positions:" << endl;
    for (const auto& pos : validPositions) {
        cout << get<0>(pos) << ", " << get<1>(pos) << ", " << get<2>(pos) << endl;
    }

    cout << "==========================================================================================" << endl;

    string filePathOb = "../../../Project/ObstacleNodes.txt";
    ifstream fileOb(filePathOb);

    if (!fileOb.is_open()) {
        cerr << "������ �� �� �����ϴ�.: " << filePathOb << endl;
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

            // �ߺ� Ȯ�� �� vector�� �߰�
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

// Z�� �� ������ txt���Ϸ� ����س���
// 2������ ���(�׺�޽�, ����Ʈ���̽�) ����ؼ� �׺�޽��� ��� ����Ʈ���̽��� ��ֹ��� ����ϱ�
// ���� ������ġ, �÷��̾� ��ġ(����), �׺�޽� ��ġ��, ��ֹ� ��ġ �����鿡 ���� �Լ� ���� �ʿ�


// ������
// ���Ͽ� ������ ����� �ȹ޾ƿ��� ��

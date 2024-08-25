#pragma once

#include <vector>
#include <string>

using namespace std;


class Player {
private:
	vector<vector<vector<int>>> PlayerPos;

public:
	Player();

	Player(vector<vector<vector<int>>> pp);

	vector<vector<vector<int>>> GetPlayerPos();

	~Player();
};
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

	void Move(int x, int y, int z);

	void Teleport(int x, int y, int z); //디버깅용

	~Player();
};
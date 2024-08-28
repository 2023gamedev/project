#pragma once

#include <vector>
#include <string>

using namespace std;


class Player {
private:
	vector<vector<vector<float>>> PlayerPos;

public:
	Player();

	Player(vector<vector<vector<float>>> pp);

	vector<vector<vector<float>>> GetPlayerPos();

	void Move(float x, float y, float z);

	void Teleport(float x, float y, float z); //디버깅용

	~Player();
};
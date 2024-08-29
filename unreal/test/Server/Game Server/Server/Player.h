#pragma once

#include <vector>
#include <string>

using namespace std;


class Player {
public:
	vector<vector<vector<float>>> PlayerLocation;	//서버에서 실행되니 서버가 해킹되지 않는 이상 public으로 편하게 써도 될듯하다


	Player();

	Player(vector<vector<vector<float>>> pp);


	void Move(float x, float y, float z);

	void Teleport(float x, float y, float z); //디버깅용

	~Player();
};
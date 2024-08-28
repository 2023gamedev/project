#include "Player.h"

Player::Player() 
{
	PlayerPos = vector<vector<vector<float>>>{ {{0, 0, 0}} };
}

Player::Player(vector<vector<vector<float>>> pp)
{
	PlayerPos = pp;
}

vector<vector<vector<float>>> Player::GetPlayerPos()
{
	return PlayerPos;
}

void Player::Move(float x, float y, float z)
{
	PlayerPos[0][0][0] = PlayerPos[0][0][0] + x;
	PlayerPos[0][0][1] = PlayerPos[0][0][1] + y;
	PlayerPos[0][0][2] = PlayerPos[0][0][2] + z;
}

//디버깅용
void Player::Teleport(float x, float y, float z)
{
	PlayerPos[0][0][0] = x;
	PlayerPos[0][0][1] = y;
	//PlayerPos[0][0][2] = z;
}

Player::~Player() 
{

}
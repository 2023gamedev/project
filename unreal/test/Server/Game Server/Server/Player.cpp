#include "Player.h"

Player::Player() 
{
	PlayerLocation = vector<vector<vector<float>>>{ {{0, 0, 0}} };
}

Player::Player(vector<vector<vector<float>>> pp)
{
	PlayerLocation = pp;
}


void Player::Move(float x, float y, float z)
{
	PlayerLocation[0][0][0] = PlayerLocation[0][0][0] + x;
	PlayerLocation[0][0][1] = PlayerLocation[0][0][1] + y;
	PlayerLocation[0][0][2] = PlayerLocation[0][0][2] + z;
}

//디버깅용
void Player::Teleport(float x, float y, float z)
{
	PlayerLocation[0][0][0] = x;
	PlayerLocation[0][0][1] = y;
	//PlayerPos[0][0][2] = z;
}

Player::~Player() 
{

}
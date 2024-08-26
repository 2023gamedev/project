#include "Player.h"

Player::Player() 
{
	PlayerPos = vector<vector<vector<int>>>{ {{0, 0, 0}} };
}

Player::Player(vector<vector<vector<int>>> pp) 
{
	PlayerPos = pp;
}

vector<vector<vector<int>>> Player::GetPlayerPos()
{
	return PlayerPos;
}

void Player::Move(int x, int y, int z)
{
	PlayerPos[0][0][0] = PlayerPos[0][0][0] + x;
	PlayerPos[0][0][1] = PlayerPos[0][0][1] + y;
	PlayerPos[0][0][2] = PlayerPos[0][0][2] + z;
}

//디버깅용
void Player::Teleport(int x, int y, int z)
{
	PlayerPos[0][0][0] = x;
	PlayerPos[0][0][1] = y;
	//PlayerPos[0][0][2] = z;
}

Player::~Player() 
{

}
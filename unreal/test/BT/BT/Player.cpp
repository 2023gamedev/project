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

Player::~Player() 
{

}
#include "Player.h"

Player::Player() 
{
	this->x = 0;
	this->y = 0;
	this->z = 0;

    floor = FLOOR::FLOOR_B2;
}

Player::Player(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


void Player::Move(float dx, float dy, float dz)
{
	this->x = this->x + dx;
	this->y = this->y + dy;
	this->z = this->z + dz;
}

//디버깅용
void Player::Teleport(float tx, float ty, float tz)
{
	this->x = tx;
	this->y = ty;
	this->z = tz;
}

Player::~Player() 
{
}
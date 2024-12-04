#pragma once

#include "Zombie.h"

class RunningZombie : public Zombie
{
public:
	RunningZombie();

	RunningZombie(Zombie_Data zd);

	//float GetHP() const override { return zombieHP; }
	//void SetHP(float hp) override { zombieHP = hp; }
};


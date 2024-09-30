#pragma once

#include "Zombie.h"

class NormalZombie : public Zombie
{
public:
	NormalZombie();

	NormalZombie(Zombie_Data zd);

	float GetHP() const override { return zombieHP; }
	void SetHP(float hp) override { zombieHP = hp; }
};


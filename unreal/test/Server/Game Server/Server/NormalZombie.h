#pragma once

#include "Zombie.h"

class NormalZombie : public Zombie
{
public:
	NormalZombie();

	NormalZombie(Zombie_Data zd);

	// 대입 연산자 오버로딩
	//NormalZombie& operator=(const NormalZombie& from) {
	//	cout << "NormalZombie& operator=(const NormalZombie& from)" << endl;
	//	ZombieData = from.ZombieData;
	//	zombieHP = from.zombieHP;
	//	return *this;
	//}

	float GetHP() const override { cout << "NormalZombie - GetHP()" << endl; return zombieHP; }
	void SetHP(float hp) override { zombieHP = hp; cout << "NormalZombie - SetHP(float hp)" << endl; }
};


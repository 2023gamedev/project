#pragma once
#include "Common.h"

struct ZombieData {
	int zombieID;
	float x, y, z;
	float pitch, yaw, roll;
};

class ZombieController
{
public:
	ZombieController();
	~ZombieController();

	vector<ZombieData> zombiedata;

	void addZombie(ZombieData zombie);
	void removeZombie(int zombieID);
	void setZombiePosition(ZombieData zombie);
	ZombieData* getZombiePosition(int zombieid);
};


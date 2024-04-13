#pragma once

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

	vector <ZombieData> zombiedata;

	void addZombie(ZombieData zombie);
	void removeZombie(int zombieID);
	void setZombiePosition(int zombieid, float x, float y, float z, float pitch, float yaw, float roll);
	ZombieData* getZombiePosition(int zombieid);
};


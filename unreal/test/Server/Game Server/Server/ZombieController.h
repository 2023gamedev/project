#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "Zombie.h"

class IOCP_CORE;


struct Vector3 {
	float X;
	float Y;
	float Z;

	Vector3() : X(0), Y(0), Z(0) {}
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

struct Rotator {
	float Pitch;
	float Yaw;
	float Roll;

	Rotator() : Pitch(0), Yaw(0), Roll(0) {}
	Rotator(float pitch, float yaw, float roll) : Pitch(pitch), Yaw(yaw), Roll(roll) {}
};

enum class EZombie
{
	NORMAL,
	SHOUTING,
	RUNNING,
};

enum class EZombiePatrol
{
	NOTPATROL,
	PATROLX,
	PATROLY,
};

class ZombieController
{
public:
	ZombieController(IOCP_CORE& mainServe);
	~ZombieController();

	IOCP_CORE* iocpServer;
	void SpawnZombies(int zombieID, int zombieType, Vector3 position, Rotator rotation, int patrolType, float patrolRange);

	void removeZombie(int zombieID);

	void SendZombieData(int id);
	void SendZombieUpdate(const Zombie_Data& z);
};


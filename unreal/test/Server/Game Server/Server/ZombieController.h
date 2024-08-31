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
	ZombieController(IOCP_CORE& mainServer);
	~ZombieController();

	IOCP_CORE* iocpServer;		// 이거 Main.cpp에서 선언한 IOCP_CORE 변수랑 같은 건지 물어보기!!! => 만약 맞다면 'IOCP_CORE& mainServer' 로 인자 선언한 함수들 모두 고치기!

	void SpawnZombies(int zombieID, int zombieType, Vector3 position, Rotator rotation, int patrolType, float patrolRange);

	void removeZombie(int zombieID);

	void SendZombieData(int id);
	void SendZombieUpdate(const Zombie_Data& z);
};


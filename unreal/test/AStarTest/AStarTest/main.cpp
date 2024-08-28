#include "ZombiePathfinder.h"

using namespace std;

int main()
{
 
	float startX = 988.0f, startY = 2964.0f, startZ = 952.0f;
	float goalX = 2299.0f, goalY = 3857.0f, goalZ = 952.0f;

	ZombiePathfinder pathfinder(startX, startY, startZ, goalX, goalY, goalZ);
	pathfinder.Run();

	return 0;


}


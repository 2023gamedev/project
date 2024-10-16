#include "ShoutingZombie.h"

#include "ZombiePathfinder.h"
#include "iocpServerClass.h"


ShoutingZombie::ShoutingZombie()
	: Zombie()
{
	SetHP(0);

	SetSpeed(0);

	bShouted = false;
}

ShoutingZombie::ShoutingZombie(Zombie_Data z_d)
	: Zombie(z_d)
{
	SetHP(ShoutingZombieStartHP);

	SetSpeed(ShoutingZombieSpeed);
	
	bShouted = false;
}

void ShoutingZombie::Shout(vector<Zombie*>& zombies)
{
	if (bShouted == false) {
		bShouted = true;

		cout << "샤우팅 좀비 #" << ZombieData.zombieID << " 샤우팅!!!" << endl;

		// 다른 좀비들 샤우팅 소리 포착 체크
		for (auto& zom : zombies) {
			if (abs(ZombieData.z - zom->ZombieData.z) > 500.f)	// 샤우팅 좀비와 같은 층 좀비만 검사
				continue;

			if (zom->ZombieData.zombieID == ZombieData.zombieID)	// 나 자신은 넘어가기
				continue;

			vector<vector<vector<float>>> szl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
			vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{zom->ZombieData.x, zom->ZombieData.y, zom->ZombieData.z}} };

			float dist = sqrt(powf(szl[0][0][0] - zl[0][0][0], 2) + powf(szl[0][0][1] - zl[0][0][1], 2)/* + powf(szl[0][0][2] - zl[0][0][2], 2)*/);	// x, y 좌표 거리만 검사

			if (dist <= CanHearShoutDistance) {
				zom->HeardShouting = true;

				zom->ShoutingLocation = szl;
			}

		}

		HaveToWait = true;	// 좀비 BT 대기상태로 변경

		animStartTime = std::chrono::high_resolution_clock::now();		// 좀비 샤우팅 시작 시간
	}
}
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

		cout << "������ ���� #" << ZombieData.zombieID << " ������!!!" << endl;

		// �ٸ� ����� ������ �Ҹ� ���� üũ
		for (auto& zom : zombies) {
			if (abs(ZombieData.z - zom->ZombieData.z) > 500.f)	// ������ ����� ���� �� ���� �˻�
				continue;

			if (zom->ZombieData.zombieID == ZombieData.zombieID)	// �� �ڽ��� �Ѿ��
				continue;

			vector<vector<vector<float>>> szl = vector<vector<vector<float>>>{ {{ZombieData.x, ZombieData.y, ZombieData.z}} };
			vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{zom->ZombieData.x, zom->ZombieData.y, zom->ZombieData.z}} };

			float dist = sqrt(powf(szl[0][0][0] - zl[0][0][0], 2) + powf(szl[0][0][1] - zl[0][0][1], 2)/* + powf(szl[0][0][2] - zl[0][0][2], 2)*/);	// x, y ��ǥ �Ÿ��� �˻�

			if (dist <= CanHearShoutDistance) {
				zom->HeardShouting = true;

				zom->ShoutingLocation = szl;
			}

		}

		HaveToWait = true;	// ���� BT �����·� ����

		animStartTime = std::chrono::high_resolution_clock::now();		// ���� ������ ���� �ð�
	}
}
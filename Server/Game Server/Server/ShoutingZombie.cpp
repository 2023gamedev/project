#include "ShoutingZombie.h"

#include "ZombiePathfinder.h"
#include "iocpServerClass.h"


ShoutingZombie::ShoutingZombie()
	: Zombie()
{
	bShouted = false;

}

ShoutingZombie::ShoutingZombie(IOCP_CORE* mainServer, Zombie_Data z_d)
	: Zombie(mainServer, z_d)
{
	ZombieStartHP = ShoutingZombieStartHP;
	SetHP(ZombieStartHP);

	//SetSpeed(ShoutingZombieSpeed);

	SetZombieType(ZOMBIE_TYPE::SHOUTING_ZOMBIE);
	
	bShouted = false;

}

void ShoutingZombie::Shout(int playerid, int roomid)
{
	if (bShouted == false) {
		bShouted = true;

#if defined(ENABLE_BT_LOG) || defined(ENABLE_BT_NODE_LOG)
		cout << "샤우팅 좀비 '#" << ZombieData.zombieID << "' 샤우팅!!!" << endl;
		cout << endl;
#endif

		vector<Zombie*> zombies = iocpServer->zombieDB_BT[roomid];

		// 다른 좀비들 샤우팅 소리 포착 체크 (샤우팅 알리기)
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

		IsShouting = true;	// 좀비 샤우팅중으로 변경

		HaveToWait = true;	// 좀비 BT 대기상태로 변경

		waitAnimStartTime = std::chrono::high_resolution_clock::now();		// 좀비 샤우팅 시작 시간

		Protocol::Zombie_shouting shoutingpacket;

		shoutingpacket.set_packet_type(25);
		shoutingpacket.set_playerid(playerid);
		shoutingpacket.set_zombieid(ZombieData.zombieID);

		string serializedData;
		shoutingpacket.SerializeToString(&serializedData);

		for (const auto& player : playerDB[roomid]) {
			//if (player.first != playerid) {	//=> 샤우팅 방식이 바껴서 이제 자신한테도 알려줘야 함 (애니메이션 재생)
				iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
			//}
		}
		
	}
}
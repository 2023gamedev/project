#include "ZombieController.h"

ZombieController::ZombieController(IOCP_CORE& mainServer)
{
    iocpServer = &mainServer;

    // 지하 1층 

    SpawnZombies(0, 0, Vector3(1800.f, 600.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);
    SpawnZombies(1, 0, Vector3(1000.f, 600.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);
    //SpawnZombies(2, 0, Vector3(600.f, 600.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);


    //SpawnZombies(3, 0, Vector3(2100.f, 2400.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, -800.f);
    //SpawnZombies(4, 0, Vector3(1200.f, 2710.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, -700.f);
    //SpawnZombies(5, 0, Vector3(400.f, 3800.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, 1200.f);
    
    
    //SpawnZombies(6, 0, Vector3(1320.f, 3100.f, 1040.275f), Rotator(0.f, -90.f, 0.f), 0, 0.f);
    //SpawnZombies(7, 0, Vector3(1400.f, 2800.f, 1040.275f), Rotator(0.f, 30.f, 0.f), 0, 0.f);
    //SpawnZombies(8, 0, Vector3(1000.f, 3320.f, 1040.275f), Rotator(0.f, 10.f, 0.f), 0, 0.f);
    //SpawnZombies(9, 0, Vector3(2200.f, 3100.f, 1040.275f), Rotator(0.f, 90.f, 0.f), 0, 0.f);
  

    //// 지상 1층
    //SpawnZombies(11, 1, Vector3(600.f, 600.f, 1989.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    //SpawnZombies(12, 1, Vector3(1000.f, 2600.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //
    //SpawnZombies(13, 0, Vector3(220.f, 1200.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(14, 0, Vector3(250.f, 1700.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(15, 0, Vector3(200.f, 1780.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(16, 0, Vector3(2100.f, 3500.f, 1989.212f), Rotator(0.f, 120.f, 0.f), 0, 0.f);
    //SpawnZombies(17, 0, Vector3(200.f, 3200.f, 1989.212f), Rotator(0.f, 20.f, 0.f), 0, 0.f);
    //SpawnZombies(18, 0, Vector3(800.f, 3600.f, 1989.212f), Rotator(0.f, -30.f, 0.f), 0, 0.f);
    //SpawnZombies(19, 0, Vector3(800.f, 1600.f, 1989.212f), Rotator(0.f, 90.f, 0.f), 0, 0.f);
    //SpawnZombies(20, 0, Vector3(200.f, 2200.f, 1989.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    
    
    ////지상 2층
    /*SpawnZombies(21, 2, Vector3(2200.f, 600.f, 2947.212f), Rotator(0.f, 90.f, 0.f), 2, 1000.f);
    SpawnZombies(22, 2, Vector3(670.f, 400.f, 2947.212f), Rotator(0.f, 90.f, 0.f), 2, 1000.f);
    SpawnZombies(23, 2, Vector3(1200.f, 1820.f, 2947.212f), Rotator(0.f, -90.f, 0.f), 0, 0.f);*/
    
    
    //SpawnZombies(24, 0, Vector3(580.f, 2170.f, 2947.212f), Rotator(0.f, -180.f, 0.f), 0, 0.f);
    //SpawnZombies(25, 0, Vector3(1850.f, 2300.f, 2947.212f), Rotator(0.f, 20.f, 0.f), 0, 0.f);
    //SpawnZombies(26, 0, Vector3(2000.f, 2800.f, 2947.212f), Rotator(0.f, -130.f, 0.f), 0, 0.f);
    //SpawnZombies(27, 0, Vector3(1400.f, 3000.f, 2947.212f), Rotator(0.f, 180.f, 0.f), 1, -800.f);
    //SpawnZombies(28, 0, Vector3(870.f, 3660.f, 2947.212f), Rotator(0.f, -90.f, 0.f), 0, 0.f);
    //SpawnZombies(29, 0, Vector3(2241.f, 3500.f, 2947.212f), Rotator(0.f, 30.f, 0.f), 0, 0.f);
    //SpawnZombies(30, 0, Vector3(190.f, 2600.f, 2947.212f), Rotator(0.f, -30.f, 0.f), 0, 0.f);
    //
    //// 지하 2층
    //SpawnZombies(31, 2, Vector3(600.f, 500.f, 90.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(32, 2, Vector3(2000.f, 1340.f, 90.212f), Rotator(0.f, 180.f, 0.f), 1, -1200.f);
    //SpawnZombies(33, 2, Vector3(2000.f, 3600.f, 90.212f), Rotator(0.f, -50.f, 0.f), 0, 0.f);
    //SpawnZombies(34, 2, Vector3(180.f, 2400.f, 90.212f), Rotator(0.f, 90.f, 0.f), 2, 800.f);
    //SpawnZombies(35, 2, Vector3(1000.f, 2800.f, 90.212f), Rotator(0.f, -60.f, 0.f), 0, 0.f);
    //
    //SpawnZombies(36, 0, Vector3(600.f, 3600.f, 90.212f), Rotator(0.f, -130.f, 0.f), 0, 0.f);
    //SpawnZombies(37, 0, Vector3(570.f, 2120.f, 90.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    //SpawnZombies(38, 0, Vector3(1600.f, 2000.f, 90.212f), Rotator(0.f, -120.f, 0.f), 0, 0.f);
    //SpawnZombies(39, 0, Vector3(200.f, 200.f, 90.212f), Rotator(0.f, -20.f, 0.f), 0, 0.f);
    //SpawnZombies(40, 0, Vector3(800.f, 2800.f, 90.212f), Rotator(0.f, 60.f, 0.f), 0, 0.f);
}

ZombieController::~ZombieController()
{
}

void ZombieController::SpawnZombies(int zombieID, int zombieType, Vector3 position, Rotator rotation, int patrolType, float patrolRange) 
{
    Zombie_Data new_zombie_data;

    new_zombie_data.zombieID = zombieID;
    new_zombie_data.x = position.X;
    new_zombie_data.y = position.Y;
    new_zombie_data.z = position.Z;
    new_zombie_data.pitch = rotation.Pitch;
    new_zombie_data.yaw = rotation.Yaw;
    new_zombie_data.roll = rotation.Roll;
    new_zombie_data.zombietype = zombieType;
    new_zombie_data.patroltype = patrolType;
    new_zombie_data.patrolrange = patrolRange;

    //플레이어 초기 위치
    vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{1850.f, 2500.f,952.f}} };
    //좀비 초기 위치
    vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{position.X, position.Y, position.Z}} };

    //좀비 인스턴스
    Zombie new_zombie = Zombie(new_zombie_data, zl);

    // zombiedata 벡터에 추가
    iocpServer->zombie.emplace_back(new_zombie);
  

    //cout << "Spawned Zombie ID: " << zombieID << ", zombiedata size: " << iocpServer->zombie.size() << endl;
}

void ZombieController::SendZombieData(int id)
{
    Protocol::ZombieDataList zombieDataList;

    for (const auto& z : iocpServer->zombie) {
        Protocol::Zombie* zombie = zombieDataList.add_zombies();
        zombie->set_zombieid(z.ZombieData.zombieID);
        zombie->set_x(z.ZombieData.x);
        zombie->set_y(z.ZombieData.y);
        zombie->set_z(z.ZombieData.z);
        zombie->set_pitch(z.ZombieData.pitch);
        zombie->set_yaw(z.ZombieData.yaw);
        zombie->set_roll(z.ZombieData.roll);
        zombie->set_zombietype(z.ZombieData.zombietype);
        //printf("%d\n", z.ZombieData.zombieID);
        //printf("%d\n", z.ZombieData.zombietype);
    }
    zombieDataList.set_packet_type(8);

    std::string serializedData;
    zombieDataList.SerializeToString(&serializedData);

    iocpServer->IOCP_SendPacket(id, serializedData.data(), serializedData.size());
}

void ZombieController::SendZombieUpdate(const Zombie_Data& z)
{
    Protocol::Zombie zombie;

    zombie.set_zombieid(z.zombieID);
    zombie.set_x(z.x);
    zombie.set_y(z.y);
    zombie.set_z(z.z);
    zombie.set_pitch(z.pitch);
    zombie.set_yaw(z.yaw);
    zombie.set_roll(z.roll);
    zombie.set_zombietype(z.zombietype);

    // 메시지를 직렬화하여 바이트 스트림으로 변환
    std::string serializedData;
    zombie.SerializeToString(&serializedData);

    // 직렬화된 데이터를 클라이언트로 전송
    for (const auto& player : g_players) {
        if (player.second->isInGame) {
            iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
    }
}

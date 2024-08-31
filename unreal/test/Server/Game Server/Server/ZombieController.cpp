#include "ZombieController.h"

ZombieController::ZombieController(IOCP_CORE& mainServer)
{
    // ���� 1�� 
    SpawnZombies(mainServer, 0, 0, Vector3(1976.f, 1976.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);
    SpawnZombies(mainServer, 1, 0, Vector3(1000.f, 600.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);
    SpawnZombies(mainServer, 2, 0, Vector3(600.f, 600.f, 1040.275f), Rotator(0.f, 0.f, 0.f), 2, 1200.f);
    
    SpawnZombies(mainServer, 3, 0, Vector3(2100.f, 2400.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, -800.f);
    SpawnZombies(mainServer, 4, 0, Vector3(1200.f, 2710.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, -700.f);
    SpawnZombies(mainServer, 5, 0, Vector3(400.f, 3800.f, 1050.275f), Rotator(0.f, 0.f, 0.f), 1, 1200.f);
    
    
    SpawnZombies(mainServer, 6, 0, Vector3(1320.f, 3100.f, 1040.275f), Rotator(0.f, -90.f, 0.f), 0, 0.f);
    SpawnZombies(mainServer, 7, 0, Vector3(1400.f, 2800.f, 1040.275f), Rotator(0.f, 30.f, 0.f), 0, 0.f);
    SpawnZombies(mainServer, 8, 0, Vector3(1000.f, 3320.f, 1040.275f), Rotator(0.f, 10.f, 0.f), 0, 0.f);
    SpawnZombies(mainServer, 9, 0, Vector3(2200.f, 3100.f, 1040.275f), Rotator(0.f, 90.f, 0.f), 0, 0.f);
  

    //// ���� 1��
    //SpawnZombies(10, 1, Vector3(600.f, 600.f, 1989.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    //SpawnZombies(11, 1, Vector3(1000.f, 2600.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //
    //SpawnZombies(12, 0, Vector3(220.f, 1200.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(13, 0, Vector3(250.f, 1700.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(14, 0, Vector3(200.f, 1780.f, 1989.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(15, 0, Vector3(2100.f, 3500.f, 1989.212f), Rotator(0.f, 120.f, 0.f), 0, 0.f);
    //SpawnZombies(16, 0, Vector3(200.f, 3200.f, 1989.212f), Rotator(0.f, 20.f, 0.f), 0, 0.f);
    //SpawnZombies(17, 0, Vector3(800.f, 3600.f, 1989.212f), Rotator(0.f, -30.f, 0.f), 0, 0.f);
    //SpawnZombies(18, 0, Vector3(800.f, 1600.f, 1989.212f), Rotator(0.f, 90.f, 0.f), 0, 0.f);
    //SpawnZombies(19, 0, Vector3(200.f, 2200.f, 1989.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    //
    //
    ////���� 2��
    //SpawnZombies(20, 2, Vector3(2200.f, 600.f, 2947.212f), Rotator(0.f, 90.f, 0.f), 2, 1000.f);
    //SpawnZombies(21, 2, Vector3(670.f, 400.f, 2947.212f), Rotator(0.f, 90.f, 0.f), 2, 1000.f);
    //SpawnZombies(22, 2, Vector3(1200.f, 1820.f, 2947.212f), Rotator(0.f, -90.f, 0.f), 0, 0.f);
    //
    //
    //SpawnZombies(23, 0, Vector3(580.f, 2170.f, 2947.212f), Rotator(0.f, -180.f, 0.f), 0, 0.f);
    //SpawnZombies(24, 0, Vector3(1850.f, 2300.f, 2947.212f), Rotator(0.f, 20.f, 0.f), 0, 0.f);
    //SpawnZombies(25, 0, Vector3(2000.f, 2800.f, 2947.212f), Rotator(0.f, -130.f, 0.f), 0, 0.f);
    //SpawnZombies(26, 0, Vector3(1400.f, 3000.f, 2947.212f), Rotator(0.f, 180.f, 0.f), 1, -800.f);
    //SpawnZombies(27, 0, Vector3(870.f, 3660.f, 2947.212f), Rotator(0.f, -90.f, 0.f), 0, 0.f);
    //SpawnZombies(28, 0, Vector3(2241.f, 3500.f, 2947.212f), Rotator(0.f, 30.f, 0.f), 0, 0.f);
    //SpawnZombies(29, 0, Vector3(190.f, 2600.f, 2947.212f), Rotator(0.f, -30.f, 0.f), 0, 0.f);
    //
    //// ���� 2��
    //SpawnZombies(30, 2, Vector3(600.f, 500.f, 90.212f), Rotator(0.f, 0.f, 0.f), 0, 0.f);
    //SpawnZombies(31, 2, Vector3(2000.f, 1340.f, 90.212f), Rotator(0.f, 180.f, 0.f), 1, -1200.f);
    //SpawnZombies(32, 2, Vector3(2000.f, 3600.f, 90.212f), Rotator(0.f, -50.f, 0.f), 0, 0.f);
    //SpawnZombies(33, 2, Vector3(180.f, 2400.f, 90.212f), Rotator(0.f, 90.f, 0.f), 2, 800.f);
    //SpawnZombies(34, 2, Vector3(1000.f, 2800.f, 90.212f), Rotator(0.f, -60.f, 0.f), 0, 0.f);
    //
    //SpawnZombies(35, 0, Vector3(600.f, 3600.f, 90.212f), Rotator(0.f, -130.f, 0.f), 0, 0.f);
    //SpawnZombies(36, 0, Vector3(570.f, 2120.f, 90.212f), Rotator(0.f, 180.f, 0.f), 0, 0.f);
    //SpawnZombies(37, 0, Vector3(1600.f, 2000.f, 90.212f), Rotator(0.f, -120.f, 0.f), 0, 0.f);
    //SpawnZombies(38, 0, Vector3(200.f, 200.f, 90.212f), Rotator(0.f, -20.f, 0.f), 0, 0.f);
    //SpawnZombies(39, 0, Vector3(800.f, 2800.f, 90.212f), Rotator(0.f, 60.f, 0.f), 0, 0.f);
}

ZombieController::~ZombieController()
{
}

void ZombieController::SpawnZombies(IOCP_CORE& mainServer, int zombieID, int zombieType, Vector3 position, Rotator rotation, int patrolType, float patrolRange) {
 
    //ZombieData zombie_data;
    Zombie_Data zombie_data;

    zombie_data.zombieID = zombieID;
    zombie_data.x = position.X;
    zombie_data.y = position.Y;
    zombie_data.z = position.Z;
    zombie_data.pitch = rotation.Pitch;
    zombie_data.yaw = rotation.Yaw;
    zombie_data.roll = rotation.Roll;
    zombie_data.zombietype = zombieType;
    zombie_data.patroltype = patrolType;
    zombie_data.patrolrange = patrolRange;

    //�÷��̾� �ʱ� ��ġ
    vector<vector<vector<float>>> pl = vector<vector<vector<float>>>{ {{1850.f, 2500.f,952.f}} };
    //���� �ʱ� ��ġ
    vector<vector<vector<float>>> zl = vector<vector<vector<float>>>{ {{position.X, position.Y, position.Z}} };

    //�÷��̾� �ν��Ͻ�
    mainServer.player = new Player(pl);
    //���� �ν��Ͻ�
    Zombie new_zombie = Zombie(zombieID, mainServer.player, zl);

    // zombiedata ���Ϳ� �߰�
    //zombiedata.push_back(zombie_data);
    mainServer.zombie.emplace_back(new_zombie);
  

    //cout << "Spawned Zombie ID: " << zombieID << ", zombiedata size: " << zombiedata.size() << endl;
    //cout << "Spawned Zombie ID: " << zombieID << ", zombiedata size: " << mainServer.zombie.size() << endl;
}

void ZombieController::SendZombieData(IOCP_CORE& mainServer, int id)
{
    Protocol::ZombieDataList zombieDataList;

    for (const auto& z : mainServer.zombie) {
        Protocol::Zombie* zombie = zombieDataList.add_zombies();
        zombie->set_zombieid(z.ZombieData.zombieID);
        zombie->set_x(z.ZombieData.x);
        zombie->set_y(z.ZombieData.y);
        zombie->set_z(z.ZombieData.z);
        zombie->set_pitch(z.ZombieData.pitch);
        zombie->set_yaw(z.ZombieData.yaw);
        zombie->set_roll(z.ZombieData.roll);
        zombie->set_zombietype(z.ZombieData.zombietype);
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

    // �޽����� ����ȭ�Ͽ� ����Ʈ ��Ʈ������ ��ȯ
    std::string serializedData;
    zombie.SerializeToString(&serializedData);

    // ����ȭ�� �����͸� Ŭ���̾�Ʈ�� ����
    for (const auto& player : g_players) {
        if (player.second->isInGame) {
            iocpServer->IOCP_SendPacket(player.first, serializedData.data(), serializedData.size());
        }
    }
}

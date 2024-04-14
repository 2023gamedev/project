#include "ZombieController.h"

ZombieController::ZombieController()
{
}

ZombieController::~ZombieController()
{
}

void ZombieController::addZombie(ZombieData zombie) {
    zombiedata.push_back(zombie);
}

void ZombieController::removeZombie(int zombieID) {
    zombiedata.erase(std::remove_if(zombiedata.begin(), zombiedata.end(),
        [zombieID](const ZombieData& z) { return z.zombieID == zombieID; }),
        zombiedata.end());
}

void ZombieController::setZombiePosition(ZombieData zombiedata)
{
    for (auto& zombie : this->zombiedata) {
        if (zombie.zombieID == zombiedata.zombieID) {
            zombie.x = zombiedata.x;
            zombie.y = zombiedata.y;
            zombie.z = zombiedata.z;
            zombie.pitch = zombiedata.pitch;
            zombie.yaw = zombiedata.yaw;
            zombie.roll = zombiedata.roll;
            break;
        }
    }
}

ZombieData* ZombieController::getZombiePosition(int zombieID) {
    for (auto& zombie : zombiedata) {
        if (zombie.zombieID == zombieID) {
            return &zombie;
        }
    }
    return nullptr;
}

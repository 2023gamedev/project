#include "ZombieController.h"
#include "Common.h"

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

void ZombieController::setZombiePosition(int zombieid, float x, float y, float z, float pitch, float yaw, float roll)
{
    for (auto& zombie : zombiedata) {
        if (zombie.zombieID == zombieid) {
            zombie.x = x;
            zombie.y = y;
            zombie.z = z;
            zombie.pitch = pitch;
            zombie.yaw = yaw;
            zombie.roll = roll;
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

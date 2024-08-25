#pragma once

#include "Task.h"

class TCanNotAttack : public Task {
public:

    bool CanSeePlayer(Zombie zom) const override {
        return zom.DistanceToPlayer > zom.CanAttackDistance;
    }

};
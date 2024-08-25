#pragma once

#include "Task.h"

class TCanAttack : public Task {
public:

    bool CanSeePlayer(Zombie zom) const override {
        return zom.DistanceToPlayer <= zom.CanAttackDistance;
    }

};
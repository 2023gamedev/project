#pragma once

#include "Task.h"

class CanAttack : public Task {
public:

    bool CanSeePlayer(Zombie zom) const override {
        return zom.DistanceToPlayer <= zom.CanAttackDistance;
    }

};
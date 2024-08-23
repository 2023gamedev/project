#pragma once

#include "Task.h"

class CanNotAttack : public Task {
public:

    bool CanSeePlayer(Zombie zom) const override {
        return  zom.DistanceToPlayer > zom.CanAttackDistance;
    }

};
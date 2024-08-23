#pragma once

#include "Task.h"

class CanAttack : public Task {
public:

    bool CanSeePlayer() const override {
        return Zombie::DistanceToPlayer <= Zombie::CanAttackDistance;
    }

};
#pragma once

#include "Task.h"

class CanNotAttack : public Task {
public:

    bool CanSeePlayer() const override {
        return Zombie::DistanceToPlayer > Zombie::CanAttackDistance;
    }

};
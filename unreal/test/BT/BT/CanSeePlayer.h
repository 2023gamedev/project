#pragma once

#include "Task.h"

class CanSeePlayer : public Task {
public:

    bool Detect() const override {
        return Zombie::DistanceToPlayer <= Zombie::CanSeeDistance;
    }

};
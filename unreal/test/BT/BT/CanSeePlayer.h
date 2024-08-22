#pragma once

#include "Task.h"
#include "Zombie.h"

class CanSeePlayer : public Task {
public:
    CanSeePlayer(float targetDistance = 10.0f) : TargetDistance(targetDistance) {}

    bool MoveTo() const override {
        return Zombie::DistanceToClosestPlayer <= TargetDistance;
    }

private:
    float TargetDistance;
};
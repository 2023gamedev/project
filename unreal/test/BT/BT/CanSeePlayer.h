#pragma once

#include "Task.h"

class CanSeePlayer : public Task {
public:

    bool Detect(Zombie zom) const override {
        return zom.DistanceToPlayer <= zom.CanSeeDistance;
    }

};
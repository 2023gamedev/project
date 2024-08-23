#pragma once

#include "Task.h"

class CanSeePlayer : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanSeeDistance);

        zom.KnewPlayerLocation = result;

        return result;
    }

};
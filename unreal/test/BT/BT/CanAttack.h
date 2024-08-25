#pragma once

#include "Task.h"

class TCanAttack : public Task {
public:

    string CanSeePlayer(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanAttackDistance);

        if (result)
            return "CanAttack-Succeed";
        else
            return "Fail";
    }

};
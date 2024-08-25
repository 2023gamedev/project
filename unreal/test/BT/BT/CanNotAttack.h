#pragma once

#include "Task.h"

class TCanNotAttack : public Task {
public:

    string CanSeePlayer(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer > zom.CanAttackDistance);

        if (result)
            return "CanNotAttack-Succeed";
        else
            return "Fail";
    }

};
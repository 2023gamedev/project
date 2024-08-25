#pragma once

#include <iostream>

#include "Task.h"


class TCanSeePlayer : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanSeeDistance);

        zom.KnewPlayerLocation = result;

        std::cout << "TCanSeePlayer의 Detect 함수 호출" << std::endl;

        return result;
    }

    //bool Detect(Zombie zom) const override { return false; };

    bool CanSeePlayer(Zombie zom) const override { return false; };

    bool CanAttack(Zombie zom) const override { return false; };

    bool CanNotAttack(Zombie zom) const override { return false; };

    bool HasShouting(Zombie zom) const override { return false; };

    bool HasFootSound(Zombie zom) const override { return false; };

    bool HasInvestigated(Zombie zom) const override { return false; };

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override { return false; };

};
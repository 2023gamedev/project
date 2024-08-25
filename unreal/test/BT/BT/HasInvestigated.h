#pragma once

#include "Task.h"

class THasInvestigated : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = zom.KnewPlayerLocation;

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
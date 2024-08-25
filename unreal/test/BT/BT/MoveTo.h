#pragma once

#include "Task.h"

class TMoveTo : public Task {
public:

    bool CanNotAttack(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.MoveTo();

        zom.Wait();

        return true;
    }

    bool HasShouting(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::SHOUTING);

        zom.MoveTo();

        zom.Wait();

        return true;
    }

    bool HasFootSound(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        zom.MoveTo();

        zom.Wait();

        return true;
    }

    bool HasInvestigated(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.MoveTo();

        zom.Wait();

        return true;
    }

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::ORIGIN);

        zom.MoveTo();

        zom.Wait();

        return true;
    }

};
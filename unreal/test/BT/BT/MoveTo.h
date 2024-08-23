#pragma once

#include "Task.h"

class MoveTo : public Task {
public:

    bool CanNotAttack(Zombie zom) const override {
        zom.MoveTo();

        return true;
    }

    bool HasInvestigated(Zombie zom) const override {
        zom.MoveTo();

        return true;
    }

    bool HasFootSound(Zombie zom) const override {
        zom.MoveTo();

        return true;
    }

    bool HasShouting(Zombie zom) const override {
        zom.MoveTo();

        return true;
    }

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override {
        zom.MoveTo();

        zom.Wait();

        zom.MoveTo();

        return true;
    }

};
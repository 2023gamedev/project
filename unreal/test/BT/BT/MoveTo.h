#pragma once

#include "Task.h"

class TMoveTo : public Task {
public:

    string CanNotAttack(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.MoveTo();

        zom.Wait();
        
        //bool result = zom.MoveTo(); 
        //if (result)
        //    return "MoveTo-Succeed";
        //else
        //    return "Fail";

        return "MoveTo-Succeed";
    }

    string HasShouting(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::SHOUTING);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string HasFootSound(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string HasInvestigated(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string NotHasLastKnownPlayerLocation(Zombie zom) const override {
        zom.SetTargetLocation(Zombie::TARGET::ORIGIN);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

};
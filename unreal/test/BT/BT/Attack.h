#pragma once

#include "Task.h"

class TAttack : public Task {
public:

    string CanAttack(Zombie zom) const override {
        zom.Attack();
        
        //bool result = zom.Attack(); 
        //if (result)
        //    return "Attack-Succeed";
        //else
        //    return "Fail";

        return "Attack-Succeed";
    }

    string Detect(Zombie zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie zom) const override { return "Fail"; };
    //string CanAttack(Zombie zom) const override { return "Fail"; };
    string CanNotAttack(Zombie zom) const override { return "Fail"; };
    string HasShouting(Zombie zom) const override { return "Fail"; };
    string HasFootSound(Zombie zom) const override { return "Fail"; };
    string HasInvestigated(Zombie zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie zom) const override { return "Fail"; };

};
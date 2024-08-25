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

};
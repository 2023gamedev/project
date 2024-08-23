#pragma once

#include "Task.h"

class Attack : public Task {
public:

    bool CanAttack(Zombie zom) const override {
        zom.Attack();
        
        return true;
    }

};
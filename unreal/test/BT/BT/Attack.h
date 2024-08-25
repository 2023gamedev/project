#pragma once

#include "Task.h"

class TAttack : public Task {
public:

    bool CanAttack(Zombie zom) const override {
        zom.Attack();
        
        return true;
    }

};
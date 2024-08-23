#pragma once

#include "Task.h"

class HasInvestigated : public Task {
public:

    bool Detect() const override {
        return Zombie::Investigated;
    }

};
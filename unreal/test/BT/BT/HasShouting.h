#pragma once

#include "Task.h"

class HasShouting : public Task {
public:

    bool Detect() const override {
        return Zombie::HeardShouting;
    }

};
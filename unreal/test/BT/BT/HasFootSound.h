#pragma once

#include "Task.h"

class HasFootSound : public Task {
public:

    bool Detect() const override {
        return Zombie::HeardFootSound;
    }

};
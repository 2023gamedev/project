#pragma once

#include "Task.h"

class NotHasLastKnownPlayerLocation : public Task {
public:

    bool Detect(Zombie zom) const override {

        return true;
    }

};
#pragma once

#include "Task.h"

class HasShouting : public Task {
public:

    bool Detect(Zombie zom) const override {
        return  zom.HeardShouting;
    }

};
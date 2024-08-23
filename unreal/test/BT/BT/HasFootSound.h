#pragma once

#include "Task.h"

class HasFootSound : public Task {
public:

    bool Detect(Zombie zom) const override {
        return  zom.HeardFootSound;
    }

};
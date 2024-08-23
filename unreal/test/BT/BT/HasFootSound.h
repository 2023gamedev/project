#pragma once

#include "Task.h"

class HasFootSound : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanHearDistance); 

        zom.HeardFootSound = result;

        return result;
    }

};
#pragma once

#include "Task.h"

class THasFootSound : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanHearDistance); 

        zom.HeardFootSound = result;

        return result;
    }

};
#pragma once

#include "Task.h"

class THasFootSound : public Task {
public:

    string Detect(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanHearDistance); 

        zom.HeardFootSound = result;

        if (result)
            return "HasFootSound-Succeed";
        else
            return "Fail";
    }

};
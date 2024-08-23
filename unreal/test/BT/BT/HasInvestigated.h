#pragma once

#include "Task.h"

class HasInvestigated : public Task {
public:

    bool Detect(Zombie zom) const override {
        bool result = zom.KnewPlayerLocation;

        return result;
    }

};
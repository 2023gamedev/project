#pragma once

#include <vector>
#include "Task.h"
#include "Zombie.h"

using namespace std;

class HasInvestigated : public Task {
private:
    bool Investigated;
    vector<vector<vector<int>>> TargetPos;

public:
    HasInvestigated() : Investigated(false), TargetPos(0) {}

    HasInvestigated(bool iv, vector<vector<vector<int>>>& tp) : Investigated(iv), TargetPos(tp) {}

    bool Detect() const override {
        return Investigated;
    }

};
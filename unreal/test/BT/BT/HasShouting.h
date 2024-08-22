#pragma once

#include <vector>
#include "Task.h"

using namespace std;

class HasShouting : public Task {
private:
    bool HeardShouting;
    vector<vector<vector<int>>> TargetPos;

public:
    HasShouting() : HeardShouting(false), TargetPos() {}

    HasShouting(bool s, vector<vector<vector<int>>>& tp) : HeardShouting(s), TargetPos(tp) {}

    bool Detect() const override {
        return HeardShouting;
    }

};
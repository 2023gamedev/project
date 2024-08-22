#pragma once

#include <vector>
#include "Task.h"

using namespace std;

class HasFootSound : public Task {
private:
    bool HeardFootSound;
    vector<vector<vector<int>>> TargetPos;

public:
    HasFootSound() : HeardFootSound(false), TargetPos() {}

    HasFootSound(bool fs, vector<vector<vector<int>>>& tp) : HeardFootSound(fs), TargetPos(tp) {}

    bool Detect() const override {
        return HeardFootSound;
    }

};
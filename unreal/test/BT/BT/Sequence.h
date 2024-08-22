#pragma once

#include "Task.h"
#include <vector>
#include <memory>

using namespace std;

class Sequence : public Task {
private:
    vector<unique_ptr<Task>> seq_children;

public:
    Sequence() = default;
    Sequence(const vector<unique_ptr<Task>>& children) : seq_children(children) {}


    bool HasInvestigated() const override {
        for (const auto& child : seq_children) {
            if (false == child->HasInvestigated()) {
                return false;
            }
        }
        return true;
    }


    void AddChild(unique_ptr<Task> child) {
        seq_children.push_back(move(child));
    }

};
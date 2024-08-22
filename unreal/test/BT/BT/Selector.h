#pragma once

#include "Task.h"
#include <vector>
#include <memory>

using namespace std;

class Selector : public Task {
private:
    vector<unique_ptr<Task>> sel_children;

public:
    Selector() = default;
    Selector(const vector<unique_ptr<Task>>& children) : sel_children(children) {}


    bool Detect() const override {
        for (const auto& child : sel_children) {
            if (true == child->Detect()) {
                return true;
            }
        }
        return false;
    }


    void AddChild(unique_ptr<Task> child) {
        sel_children.push_back(move(child));
    }

};
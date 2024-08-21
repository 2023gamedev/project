#pragma once

#include "Task.h"
#include <vector>
#include <memory>

class Sequence : public Task {
public:
    Sequence() = default;
    Sequence(const std::vector<std::unique_ptr<Task>>& children) : children(children) {}


    bool Attack() const override {
        for (const auto& child : children) {
            if (!child->Attack()) {
                return false;
            }
        }
        return true;
    }


    void AddChild(std::unique_ptr<Task> child) {
        children.push_back(std::move(child));
    }

private:
    std::vector<std::unique_ptr<Task>> children;
};
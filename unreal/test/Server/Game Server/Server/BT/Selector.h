#pragma once

#include "BT\Task.h"
#include <vector>
#include <memory>

class Selector : public Task {
public:
    // 생성자
    Selector() = default;
    Selector(const std::vector<std::unique_ptr<Task>>& children) : children(children) {}

    // Run 메서드 구현
    bool Run() const override {
        for (const auto& child : children) {
            if (child->Run()) {
                return true;
            }
        }
        return false;
    }

    // 자식 태스크 추가 메서드
    void AddChild(std::unique_ptr<Task> child) {
        children.push_back(std::move(child));
    }

private:
    std::vector<std::unique_ptr<Task>> children;
};
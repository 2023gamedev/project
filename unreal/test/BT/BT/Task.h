#pragma once

class Task {
public:
    virtual bool Run() const = 0;

    virtual ~Task() = default;
};
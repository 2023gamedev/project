#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using namespace std;


class Sequence : public Task {
private:
    vector<unique_ptr<Task>> seq_children;

public:
    Sequence() = default;
    //Sequence(const vector<unique_ptr<Task>>& children) : seq_children(children) {}


    bool CanNotAttack(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->CanNotAttack(zom)) {
                return false;
            }
        }
        return true;
    }

    bool CanAttack(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->CanAttack(zom)) {
                return false;
            }
        }
        return true;
    }

    bool HasShouting(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->HasShouting(zom)) {
                return false;
            }
        }
        return true;
    }

    bool HasFootSound(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->HasFootSound(zom)) {
                return false;
            }
        }
        return true;
    }

    bool HasInvestigated(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->HasInvestigated(zom)) {
                return false;
            }
        }
        return true;
    }

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override {
        for (const auto& child : seq_children) {
            if (false == child->NotHasLastKnownPlayerLocation(zom)) {
                return false;
            }
        }
        return true;
    }


    void AddChild(unique_ptr<Task> child) {
        seq_children.emplace_back(move(child));
    }

};
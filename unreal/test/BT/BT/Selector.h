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
    //Selector(const vector<unique_ptr<Task>>& children) : sel_children(children) {}


    bool Detect(Zombie zom) const override {
        for (const auto& child : sel_children) {
            if (true == child->Detect(zom)) {
                return true;
            }
        }
        return false;   //사실상 실패할 일은 없긴하지만
    }

    bool CanSeePlayer(Zombie zom) const override {
        for (const auto& child : sel_children) {
            if (true == child->CanSeePlayer(zom)) {
                return true;
            }
        }
        return false; //사실상 실패할 일은 없긴하지만
    }


    void AddChild(unique_ptr<Task> child) {
        sel_children.emplace_back(move(child));
    }


    //bool Detect(Zombie zom) const override {};

    //bool CanSeePlayer(Zombie zom) const override {};

    bool CanAttack(Zombie zom) const override { return false; };

    bool CanNotAttack(Zombie zom) const override { return false; };

    bool HasShouting(Zombie zom) const override { return false; };

    bool HasFootSound(Zombie zom) const override { return false; };

    bool HasInvestigated(Zombie zom) const override { return false; };

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override { return false; };

};
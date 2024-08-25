#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using namespace std;


//사실 Task의 자식 클래스로 생성 안해도 무관함. 이를 한번 나중에 다시 생각해보기.
class Sequence : public Task {
private:
    vector<unique_ptr<Task>> seq_children;

public:
    Sequence() = default;
    //Sequence(const vector<unique_ptr<Task>>& children) : seq_children(children) {}


    bool Seq_CanNotAttack(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->CanNotAttack(zom)) {
                return false;
            }
        }
        return true;
    }

    bool Seq_CanAttack(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->CanAttack(zom)) {
                return false;
            }
        }
        return true;
    }

    bool Seq_HasShouting(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->HasShouting(zom)) {
                return false;
            }
        }
        return true;
    }

    bool Seq_HasFootSound(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->HasFootSound(zom)) {
                return false;
            }
        }
        return true;
    }

    bool Seq_HasInvestigated(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->HasInvestigated(zom)) {
                return false;
            }
        }
        return true;
    }

    bool Seq_NotHasLastKnownPlayerLocation(Zombie zom) {
        for (const auto& child : seq_children) {
            if ("Fail" != child->NotHasLastKnownPlayerLocation(zom)) {
                return false;
            }
        }
        return true;
    }


    void AddChild(unique_ptr<Task> child) {
        seq_children.emplace_back(move(child));
    }

};
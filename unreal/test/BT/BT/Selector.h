#pragma once

#include <vector>
#include <memory>

#include "Task.h"
#include "CanSeePlayer.h"

using namespace std;


class Selector : public Task {
public:
    vector<unique_ptr<Task>>* sel_children = new vector<unique_ptr<Task>>;

public:
    Selector() = default;
    Selector(TCanSeePlayer task) { cout << "TCanSeePlayer task 형식을 받는 Selector 생성자" << endl; }
    //Selector(vector<unique_ptr<Task>>* children) : sel_children(children) { cout << "vector<unique_ptr<Task>>* children 형식을 받는 Selector 생성자" << endl; }


    bool Sel_Detect(Zombie zom) {
        cout << "Selector의 Sel_Detect 함수 호출" << endl;
        for (const auto& child : *sel_children) {
            //cout << typeid(child).name() << endl;
            if (true == child->Detect(zom)) {
                return true;
            }
        }
        return false;   //사실상 실패할 일은 없긴하지만
    }

    bool Sel_CanSeePlayer(Zombie zom) {
        for (const auto& child : *sel_children) {
            if (true == child->CanSeePlayer(zom)) {
                return true;
            }
        }
        return false; //사실상 실패할 일은 없긴하지만
    }


    void AddChild(Task* child) {
        sel_children->emplace_back(move(child));
    }


    bool Detect(Zombie zom) const override { return false; };

    bool CanSeePlayer(Zombie zom) const override { return false; };

    bool CanAttack(Zombie zom) const override { return false; };

    bool CanNotAttack(Zombie zom) const override { return false; };

    bool HasShouting(Zombie zom) const override { return false; };

    bool HasFootSound(Zombie zom) const override { return false; };

    bool HasInvestigated(Zombie zom) const override { return false; };

    bool NotHasLastKnownPlayerLocation(Zombie zom) const override { return false; };

};
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


    string Seq_CanNotAttack(Zombie zom) {
        cout << "Sequence의 {Seq_CanNotAttack} 함수 호출" << endl;
        cout << endl;
        for (const auto& child : seq_children) {
            string result = child->CanNotAttack(zom);
            if ("Fail" != result) {
                cout << "\"{Sequence CanNotAttack}의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
                return result;
            }
        }
        cout << "\"Sequence CanNotAttack [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";   //사실상 실패할 일은 없긴하지만
    }

    string Seq_CanAttack(Zombie zom) {
        cout << "Sequence의 {Seq_CanAttack} 함수 호출" << endl;
        cout << endl;
        for (const auto& child : seq_children) {
            string result = child->CanAttack(zom);
            if ("Fail" != result) {
                cout << "\"{Sequence CanAttack}의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
                return result;
            }
        }
        cout << "\"Sequence CanAttack [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";      //이건 실패 할 수 있음 (여기가 공격을 실행하는 Task인데 좀비의 공격이 벽에 막히는 경우를 생각해본다면)
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

    string Seq_HasInvestigated(Zombie zom) {
        cout << "Sequence의 {Seq_HasInvestigated} 함수 호출" << endl;
        cout << endl;
        for (const auto& child : seq_children) {
            string result = child->HasInvestigated(zom);
            if ("Fail" != result) {
                cout << "\"{Sequence HasInvestigated}의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
                return result;
            }
        }
        cout << "\"Sequence HasInvestigated [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";   //사실상 실패할 일은 없긴하지만
    }

    string Seq_NotHasLastKnownPlayerLocation(Zombie zom) {
        cout << "Sequence의 {Seq_NotHasLastKnownPlayerLocation} 함수 호출" << endl;
        cout << endl;
        for (const auto& child : seq_children) {
            string result = child->NotHasLastKnownPlayerLocation(zom);
            if ("Fail" != result) {
                cout << "\"{Sequence NotHasLastKnownPlayerLocation}의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
                return result;
            }
        }
        cout << "\"Sequence NotHasLastKnownPlayerLocation [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";   //사실상 실패할 일은 없긴하지만
    }


    void AddChild(Task* child) {
        seq_children.emplace_back(move(child));
    }


    string Detect(Zombie zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie zom) const override { return "Fail"; };
    string CanAttack(Zombie zom) const override { return "Fail"; };
    string CanNotAttack(Zombie zom) const override { return "Fail"; };
    string HasShouting(Zombie zom) const override { return "Fail"; };
    string HasFootSound(Zombie zom) const override { return "Fail"; };
    string HasInvestigated(Zombie zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie zom) const override { return "Fail"; };

};
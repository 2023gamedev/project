#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using std::vector;
using std::unique_ptr;


//사실 Task의 자식 클래스로 생성 안해도 무관함. 이를 한번 나중에 다시 생각해보기.
class Sequence : public Task {
private:
    vector<unique_ptr<Task>> seq_children;      //근데, 굳이 unique_ptr을 사용해야 할...까? 고민 => C++에서 객체를 벡터에 직접 저장할 때, '슬라이스 현상'이 발생하며 해당 '스마트 포인터'는 이를 방지해줌

public:
    Sequence() = default;


    bool Seq_CanAttack(Zombie& zom) {
        //cout << "Sequence {CanAttack} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->CanAttack(zom);
        }

        if (result == false) {
            cout << "\"Sequence CanAttack [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;      //이건 실패 할 수 있음 (여기가 공격을 실행하는 Task인데 좀비의 공격이 벽에 막히는 경우를 생각해본다면) 
                            // [x] -> 공격 명령만 주고 실제 충돌체크는 클라에서 체크하고 결과 보내줌 (그래서 지금 시퀀스 CanAttack의 유일한 Task인 Attack에서 무조건 성공 리턴하게 만들어 놓음)
    }

    bool Seq_CanNotAttack(Zombie& zom) {
        //cout << "Sequence {CanNotAttack} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->CanNotAttack(zom);
        }

        if (result == false) {
            cout << "\"Sequence CanNotAttack [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

    bool Seq_HasShouting(Zombie& zom) {
        //cout << "Sequence {HasShouting} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->HasShouting(zom);
        }

        if (result == false) {
            cout << "\"Sequence HasShouting [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

    bool Seq_HasFootSound(Zombie& zom) {
        //cout << "Sequence {HasFootSound} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->HasFootSound(zom);
        }

        if (result == false) {
            cout << "\"Sequence HasFootSound [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

    bool Seq_HasInvestigated(Zombie& zom) {
        //cout << "Sequence {HasInvestigated} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->HasInvestigated(zom);
        }

        if (result == false) {
            cout << "\"Sequence HasInvestigated [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

    bool Seq_NotHasLastKnownPlayerLocation(Zombie& zom) {
        //cout << "Sequence {NotHasLastKnownPlayerLocation} 함수 호출" << endl;
        //cout << endl;

        for (const auto& child : seq_children) {
            result = child->NotHasLastKnownPlayerLocation(zom);
        }

        if (result == false) {
            cout << "\"Sequence NotHasLastKnownPlayerLocation [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }


    void AddChild(Task* child) {
        seq_children.emplace_back(move(child));
    }

};
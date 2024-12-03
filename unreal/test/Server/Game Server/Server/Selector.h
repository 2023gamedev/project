#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using std::vector;
using std::unique_ptr;


//사실 Task의 자식 클래스로 생성 안해도 무관함. 이를 한번 나중에 다시 생각해보기.
class Selector : public Task {
private:
    vector<unique_ptr<Task>> sel_children;      //근데, 굳이 unique_ptr을 사용해야 할...까? 고민 => C++에서 객체를 벡터에 직접 저장할 때, '슬라이스 현상'이 발생하며 해당 '스마트 포인터'는 이를 방지해줌

public:
    Selector() = default;


    string Sel_Detect(Zombie& zom) {
        //cout << "Selector <Detect> 호출" << endl;
        //cout << endl;
        for (const auto& child : sel_children) {
            string result = child->Detect(zom); // 다형성 이용 (함수 오버라이딩)
            if ("Fail" != result) {
#ifdef	ENABLE_BT_LOG
                cout << "\"<Selector Detect>의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
#endif
                return result;
            }
        }
        cout << "\"Selector Detect [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";   //사실상 실패할 일은 없긴하지만
    }

    string Sel_CanSeePlayer(Zombie& zom) {
        //cout << "Selector <CanSeePlayer> 함수 호출" << endl;
        //cout << endl;
        for (const auto& child : sel_children) {
            string result = child->CanSeePlayer(zom);
            if ("Fail" != result) {
#ifdef	ENABLE_BT_LOG
                cout << "\"<Selector CanSeePlayer>의 Task 중 [" << result << "]!!!\"" << endl;
                cout << endl;
#endif
                return result;
            }
        }
        cout << "\"Selector CanSeePlayer [ERROR]!!!\"" << endl;
        cout << endl;
        return "Fail";   //사실상 실패할 일은 없긴하지만
    }


    void AddChild(Task* child) {
        sel_children.emplace_back(move(child));
    }


    //사실상 더미 함수들
    string Detect(Zombie& zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    string CanAttack(Zombie& zom) const override { return "Fail"; };
    string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    string HasShouting(Zombie& zom) const override { return "Fail"; };
    string HasFootSound(Zombie& zom) const override { return "Fail"; };
    string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};
#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using std::vector;
using std::unique_ptr;


//사실 Task의 자식 클래스로 생성 안해도 무관함. 이를 한번 나중에 다시 생각해보기.
class Selector : public Task {
private:
    vector<unique_ptr<Task>> sel_children;  //근데, 굳이 unique_ptr을 사용해야 할...까? 고민

public:
    Selector() = default;


    string Sel_Detect(Zombie& zom) {
        //cout << "Selector <Detect> 호출" << endl;
        //cout << endl;
        for (const auto& child : sel_children) {
            string result = child->Detect(zom);
            if ("Fail" != result) {
                //cout << "\"<Selector Detect>의 Task 중 [" << result << "]!!!\"" << endl;
                //cout << endl;
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
            //좀비와 플레이어들의 거리 갱신
            //==============================================거리로만 공격을 판단하지 말고 방향 벡터까지 계산해서 공격 명령시키도록 하자 => 안해도 됨 (이미 Detect에서 시야 검사 되고 실행되니)
            //zom.SetDistance();

            string result = child->CanSeePlayer(zom);
            if ("Fail" != result) {
                //cout << "\"<Selector CanSeePlayer>의 Task 중 [" << result << "]!!!\"" << endl;
                //cout << endl;
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
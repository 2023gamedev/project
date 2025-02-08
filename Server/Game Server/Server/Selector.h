#pragma once

#include <vector>
#include <memory>

#include "Task.h"

using std::vector;
using std::unique_ptr;


//사실 Task의 자식 클래스로 생성 안해도 무관함. 이를 한번 나중에 다시 생각해보기.
class Selector : public Task {
public:
    vector<unique_ptr<Task>> sel_children;      //근데, 굳이 unique_ptr을 사용해야 할...까? 고민 => C++에서 객체를 벡터에 직접 저장할 때, '슬라이스 현상'이 발생하며 해당 '스마트 포인터'는 이를 방지해줌


    bool Sel_Detect(Zombie& zom) {
        //cout << "Selector <Detect> 호출" << endl;
        //cout << endl;

        result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->Detect(zom);  // 다형성 이용 (함수 오버라이딩)

            if (result == false)  
                result = task_result; 
        }

        if (result == false) {  // selector의 모든 task가 실패 할 경우(그럴 일은 없어야 하지만..)
            cout << "\"Selector Detect [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

    bool Sel_CanSeePlayer(Zombie& zom) {
        //cout << "Selector <CanSeePlayer> 함수 호출" << endl;
        //cout << endl;

        result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->CanSeePlayer(zom);    // 다형성 이용 (함수 오버라이딩)

            if (result == false)
                result = task_result;

            if (result == true) // 최적화(먼저 한가지 task가 걸리면 해당 task 선택하고 바로 종료함) -> Selector 병렬적으로 사용하려면 해당 코드 지워야함 
                return result;
        }

        if (result == false) {  // selector의 모든 task가 실패 할 경우(그럴 일은 없어야 하지만..)
            cout << "\"Selector CanSeePlayer [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result; 
    }


    void AddChild(Task* child) {
        sel_children.emplace_back(move(child));
    }

};
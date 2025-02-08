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


    void AddChild(Task* child) {
        sel_children.emplace_back(move(child));
    }

};
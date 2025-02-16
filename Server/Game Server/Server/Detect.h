#pragma once

#include "Selector.h"


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Selector <Detect> 호출" << endl;
        cout << endl;
#endif

        d_result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->Detect(zom);  // 다형성 이용 (함수 오버라이딩)

            if (d_result == false)
                d_result = task_result;

            if (d_result == true)
                break;  // SetTargetLocation이 문제라 병렬로 돌리면 현재로써는 안 됨...;; (TargetLocation 자꾸 덮어씌우기함)
        }

        if (d_result == false) {  // selector의 모든 task가 실패 할 경우(그럴 일은 없어야 하지만..)
            cout << "\"Selector Detect [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
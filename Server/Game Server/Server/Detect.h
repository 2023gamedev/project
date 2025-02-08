#pragma once

#include "Selector.h"


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect> 호출" << endl;
        cout << endl;
#endif

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

};
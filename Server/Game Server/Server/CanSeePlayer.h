#pragma once

#include "Selector.h"


class Sel_CanSeePlayer : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (CanSeePlayer Decorator) 호출" << endl;
#endif

        result = zom.PlayerInSight_Update_Check();

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 플레이어가 있는가?: " << boolalpha << result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (CanSeePlayer Decorator) 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
#endif

        if (result == true)
            CanSeePlayer(zom);

        return result;
    }

    bool CanSeePlayer(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Selector <CanSeePlayer> 호출" << endl;
        cout << endl;
#endif

        result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->CanSeePlayer(zom);    // 다형성 이용 (함수 오버라이딩)

            if (result == false)
                result = task_result;
        }

        if (result == false) {  // selector의 모든 task가 실패 할 경우(그럴 일은 없어야 하지만..)
            cout << "\"Selector CanSeePlayer [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }
};
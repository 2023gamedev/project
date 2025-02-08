#pragma once

#include "Selector.h"


class Sel_CanSeePlayer : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 [CanSeePlayer Task] 호출" << endl;
#endif

        result = zom.PlayerInSight_Update_Check();

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 플레이어가 있는가?: " << boolalpha << result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [CanSeePlayer Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
#endif

        return result;
    }

};
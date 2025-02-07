#pragma once

#include <iostream>

#include "Selector.h"
#include "iocpServerClass.h"

using std::cout;
using std::endl;


class Sel_CanSeePlayer : public Selector {
public:

    bool Detect(Zombie& zom) override {
        //cout << "<Detect>의 [CanSeePlayer Task] 호출" << endl;

        result = zom.PlayerInSight_Update_Check();

        //cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 플레이어가 있는가?: " << boolalpha << result << endl;
        //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [CanSeePlayer Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        return result;
    }

};
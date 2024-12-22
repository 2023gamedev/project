#pragma once

#include <iostream>

#include "Task.h"
#include "iocpServerClass.h"

using std::cout;
using std::endl;


class TCanSeePlayer : public Task {
public:

    string Detect(Zombie& zom) const override {
        //cout << "<Detect>의 [CanSeePlayer Task] 호출" << endl;

        bool result = zom.PlayerInSight;

        //cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 플레이어가 있는가?: " << boolalpha << result << endl;
        //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [CanSeePlayer Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;
        
        if (result)
            return "CanSeePlayer-Succeed";
        else
            return "Fail";
    }

    //사실상 더미 함수들
    //string Detect(Zombie& zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    string CanAttack(Zombie& zom) const override { return "Fail"; };
    string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    string HasShouting(Zombie& zom) const override { return "Fail"; };
    string HasFootSound(Zombie& zom) const override { return "Fail"; };
    string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};
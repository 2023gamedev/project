#pragma once

#include "Task.h"


class THasShouting : public Task {
public:

    string Detect(Zombie& zom) const override {
        //cout << "<Detect>의 [HasShouting Task] 호출" << endl;

        bool result = zom.HeardShouting;

        if (result == true) {
            zom.SetTargetLocation(Zombie::TARGET::SHOUTING);
        }

        /*cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasShouting Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;*/

        if (result)
            return "HasShouting-Succeed";
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
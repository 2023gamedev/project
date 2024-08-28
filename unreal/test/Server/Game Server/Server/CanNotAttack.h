#pragma once

#include <iostream>

#include "Task.h"

using namespace std;


class TCanNotAttack : public Task {
public:

    string CanSeePlayer(Zombie& zom) const override {
        bool result = (zom.DistanceToPlayer > zom.CanAttackDistance);

        cout << "<CanSeePlayer>의 [CanNotAttack Task] 호출" << endl;
        cout << "플레이어와 좀비의 거리: " << zom.DistanceToPlayer << endl;
        cout << "좀비의 공격 사거리: " << zom.CanAttackDistance << endl;
        cout << "\'따라서\', <CanSeePlayer>의 [CanNotAttack Task] 결과: " << boolalpha << result << endl;
        cout << endl;

        if (result)
            return "CanNotAttack-Succeed";
        else
            return "Fail";
    }

    //사실상 더미 함수들
    string Detect(Zombie& zom) const override { return "Fail"; };
    //string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    string CanAttack(Zombie& zom) const override { return "Fail"; };
    string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    string HasShouting(Zombie& zom) const override { return "Fail"; };
    string HasFootSound(Zombie& zom) const override { return "Fail"; };
    string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};
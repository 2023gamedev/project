#pragma once

#include "Task.h"

class TCanAttack : public Task {
public:

    string CanSeePlayer(Zombie zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanAttackDistance);

        cout << "<CanSeePlayer>의 [CanAttack Task] 호출" << endl;
        cout << "플레이어와 좀비의 거리: " << zom.DistanceToPlayer << endl;
        cout << "좀비의 공격 사거리: " << zom.CanAttackDistance << endl;
        cout << "\'따라서\', [CanAttack Task]의 <CanSeePlayer> 함수 결과: " << boolalpha << result << endl;
        cout << endl;

        if (result)
            return "CanAttack-Succeed";
        else
            return "Fail";
    }

    string Detect(Zombie zom) const override { return "Fail"; };
    //string CanSeePlayer(Zombie zom) const override { return "Fail"; };
    string CanAttack(Zombie zom) const override { return "Fail"; };
    string CanNotAttack(Zombie zom) const override { return "Fail"; };
    string HasShouting(Zombie zom) const override { return "Fail"; };
    string HasFootSound(Zombie zom) const override { return "Fail"; };
    string HasInvestigated(Zombie zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie zom) const override { return "Fail"; };

};
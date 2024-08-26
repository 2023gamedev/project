#pragma once

#include <iostream>

#include "Task.h"

using namespace std;


class TNotHasLastKnownPlayerLocation : public Task {
public:

    string Detect(Zombie zom) const override {
        bool result = !zom.KnewPlayerLocation;

        cout << "<Detect>의 [NotHasLastKnownPlayerLocation Task] 호출" << endl;
        cout << "좀비 플레이어의 최신 위치 정보를 가지고 있는가: " << boolalpha << !result << endl;
        if (!result) {
            cout << "좀비가 가진 플레이어 최신 위치: ( " << zom.TargetLocation[0][0][0] << ", " << zom.TargetLocation[0][0][1] << ", " << zom.TargetLocation[0][0][2] << " )" << endl;
            cout << "사실상 이 코드는 실행되면 안됨!!!!!!!!!" << endl;
        }
        cout << "\'따라서\', <Detect>의 [NotHasLastKnownPlayerLocation Task] 결과: " << boolalpha << result << endl;
        cout << endl;

        if (result)
            return "NotHasLastKnownPlayerLocation-Succeed";
        else
            return "Fail";
    }

    //string Detect(Zombie zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie zom) const override { return "Fail"; };
    string CanAttack(Zombie zom) const override { return "Fail"; };
    string CanNotAttack(Zombie zom) const override { return "Fail"; };
    string HasShouting(Zombie zom) const override { return "Fail"; };
    string HasFootSound(Zombie zom) const override { return "Fail"; };
    string HasInvestigated(Zombie zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie zom) const override { return "Fail"; };

};
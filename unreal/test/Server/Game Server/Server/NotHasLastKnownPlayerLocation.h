#pragma once

#include "Task.h"


class TNotHasLastKnownPlayerLocation : public Task {
public:

    string Detect(Zombie& zom) const override {
        //cout << "<Detect>의 [NotHasLastKnownPlayerLocation Task] 호출" << endl;

        //bool result = !zom.KnewPlayerLocation;
        bool result = true;

        //cout << "좀비 플레이어의 최신 위치 정보를 가지고 있는가?: " << boolalpha << !result << endl;
        if (!result) {
            cout << "좀비가 가진 플레이어 최신 위치: ( " << zom.PrevTargetLocation[0][0][0] << ", " << zom.PrevTargetLocation[0][0][1] << ", " << zom.PrevTargetLocation[0][0][2] << " )" << endl;
            cout << "근데, 사실상 이 코드는 실행되면 안됨!!!!!!!!! [ERROR]" << endl;
        }
        //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [NotHasLastKnownPlayerLocation Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        if (result)
            return "NotHasLastKnownPlayerLocation-Succeed";
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
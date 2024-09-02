#pragma once

#include "Task.h"


class THasInvestigated : public Task {
public:

    string Detect(Zombie& zom) const override {
        bool result = zom.KnewPlayerLocation;

        //cout << "<Detect>의 [HasInvestigated Task] 호출" << endl;
        //cout << "좀비 플레이어의 최신 위치 정보를 가지고 있는가?: " << boolalpha << result << endl;
        //if (result)
            //cout << "좀비가 가진 플레이어 최신 위치: ( " << zom.TargetLocation[0][0][0] << ", " << zom.TargetLocation[0][0][1] << ", " << zom.TargetLocation[0][0][2] << " )" << endl;
        cout << "따라서, 플레이어 \'#" << zom.bt_playerID << "\' 에 대한 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasInvestigated Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;

        if (result)
            return "HasInvestigated-Succeed";
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
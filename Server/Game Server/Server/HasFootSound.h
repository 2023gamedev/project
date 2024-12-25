#pragma once

#include "Task.h"


class THasFootSound : public Task {
public:

    string Detect(Zombie& zom) const override {
        //cout << "<Detect>의 [HasFootSound Task] 호출" << endl;

        bool result = zom.FootSound_Update_Check();

        if (result == true) // 새로운 발소리를 들었을 경우에만 실행해서 가장 가까운 
            zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        if (zom.HeardFootSound == true) // FootSound_Update_Check 결과 상관없이 이미 근처 발소리를 들은 경우에는 -> 계속 실행
            result = true;

        //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasFootSound Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        if (result)
            return "HasFootSound-Succeed";
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
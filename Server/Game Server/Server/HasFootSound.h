#pragma once

#include "Task.h"


class THasFootSound : public Task {
public:

    bool Detect(Zombie& zom) override {
        cout << "<Detect>의 [HasFootSound Task] 호출" << endl;

        result = zom.FootSound_Update_Check();

        if (result == true) // 새로운 발소리를 들었을 경우에만 실행해서 가장 가까운 플레이어의 발소리를 목표지점으로
            zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        if (zom.HeardFootSound == true) // FootSound_Update_Check 결과 상관없이 이미 근처 발소리를 들은 경우에는(이전에 들은 발소리를 아직 기억하는 경우) -> 계속 실행
            result = true;

        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasFootSound Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        return result;
    }

};
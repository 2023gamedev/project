#pragma once

#include "Task.h"


class THasFootSound : public Task {
public:

    string Detect(Zombie& zom) const override {
        bool result = (zom.DistanceToPlayer <= zom.CanHearDistance);    //생각해 봐야 할 것: 이렇게 HasFootSound에서 조건을 검사하고 HeardFootSound를 변경하는 과정을 진행하게되면,
                                                                        // Shouting과 HeardFootSound가 "둘 다" 포착 될 때, Selector에서 Shouting "만"을 먼저 실행하게 설계되어 있기에,
                                                                        // 이를 Player에서 대신 계산해서 Zombie에게 알려주어 BT 프로세스와는 별도로 bool값을 변경해주면 좋을 것 같다.

        zom.HeardFootSound = result;

        if (result)
            return "HasFootSound-Succeed";
        else
            return "Fail";
    }

};
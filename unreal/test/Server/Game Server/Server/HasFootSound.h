#pragma once

#include "Task.h"


class THasFootSound : public Task {
public:

    string Detect(Zombie& zom) const override {
        //bool result = (zom.DistanceToPlayer <= zom.CanHearDistance);      //생각해 봐야 할 것: 이렇게 HasFootSound에서 조건을 검사하고 HeardFootSound를 변경하는 과정을 진행하게되면,
                                                                            // Shouting과 HeardFootSound가 "둘 다" 포착 될 때, Selector에서 Shouting "만"을 먼저 실행하게 설계되어 있기에,
                                                                            // 이를 Player에서 대신 계산해서 Zombie에게 알려주어 BT 프로세스와는 별도로 bool값을 변경해주면 좋을 것 같다.

        //zom.HeardFootSound = result;

        //생각해보니 여기서 검사 할 필요가 없음 
        // -> 클라에서 플레이어가 뛰었을때 주변 좀비들과 거리 계산(검사)해서 들었다면, 서버로 해당 좀비들 HeardFootSound 패킷 서버로 전송 
        // -> 서버 클라로부터 HeardFootSound 패킷받으면, 해당 좀비들 HeardFootSound을 true로 바꾼다! (CanSeePlayer와 같은 방식으로)

        bool result = zom.HeardFootSound;

        cout << "따라서, 플레이어 \'#" << zom.bt_playerID << "\' 에 대한 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasFootSound Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;

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
#pragma once

#include "Task.h"


class THasShouting : public Task {
public:

    string Detect(Zombie& zom) const override {
        //샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요

        //bool result = (zom.DistanceToPlayer <= zom.CanHearShoutDistance); //DistanceToSZombie: 샤우팅 좀비로 부터의 거리, CanHearShoutDistance: 샤우팅을 들을 수 있는 거리 (const 변수)

        //zom.HeardShouting = result;

        //생각해보니 여기서 검사 할 필요가 없음 
        // -> 클라에서 샤우팅 좀비가 플레이어를 발견해서 샤우팅을 하면 Shouting 패킷 서버로 전송 & 주변 좀비들 샤우팅을 들었는지 안들었는지 거리 계산(검사)해서 HeardShouting 패킷 서버로 전송 
        // -> 서버 클라로부터 HeardShouting 패킷받으면, 해당 좀비들 HeardShouting을 true로 바꾼다! (CanSeePlayer와 같은 방식으로)
        // -> 서버에서 Shouting 패킷을 받으면, 서버에 해당 샤우팅 좀비 Shouting bool 변수를 true 바꾸기 (만약 다른 클라에서 또 Shouting 패킷을 보내도 Shouting을 두번 하지 못하게 막기위해)
        // 다만 여기서 샤우팅 좀비의 위치도 따로 검색해 받아서 좀비들에게 넘겨줘야함 => Zombie.cpp에 SetTargetLocation(TARGET t)에서 작업해야함

        bool result = zom.HeardShouting;

        cout << "따라서, 플레이어 \'#" << zom.bt_playerID << "\' 에 대한 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasShouting Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;

        if (result)
            return "HasShouting - Succeed";
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
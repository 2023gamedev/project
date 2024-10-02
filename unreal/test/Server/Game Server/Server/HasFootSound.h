#pragma once

#include "Task.h"


class THasFootSound : public Task {
public:

    string Detect(Zombie& zom) const override {
        //cout << "<Detect>의 [HasFootSound Task] 호출" << endl;

        //bool result = (zom.DistanceToPlayer <= zom.CanHearDistance);      //생각해 봐야 할 것: 이렇게 HasFootSound에서 조건을 검사하고 HeardFootSound를 변경하는 과정을 진행하게되면,
                                                                            // Shouting과 HeardFootSound가 "둘 다" 포착 될 때, Selector에서 Shouting "만"을 먼저 실행하게 설계되어 있기에,
                                                                            // 이를 Player에서 대신 계산해서 Zombie에게 알려주어 BT 프로세스와는 별도로 bool값을 변경해주면 좋을 것 같다.
                                                                            // -> 이러면 데이터 레이스 생길 우려가 있으니 걍 여기서 계산하기로
                                                                            // => 근데 이거 타게팅에대해서 회의가 필요해보임 
                                                                            // (만약 둘 다 들었다면 지금은 샤우팅이 우선이니까 발소리는 걍 무시하는걸로? 
                                                                            // 아니면,
                                                                            //  그래도 나중에 샤우팅 지점 도착하고 나서 발소리 지점으로 가도록 만들기?)

        //zom.HeardFootSound = result;

        //생각해보니 여기서 검사 할 필요가 없음 
        // -> 클라에서 플레이어가 뛰었을때 주변 좀비들과 거리 계산(검사)해서 들었다면, 서버로 해당 좀비들 HeardFootSound 패킷 서버로 전송 
        // -> 서버 클라로부터 HeardFootSound 패킷받으면, 해당 좀비들 HeardFootSound을 true로 바꾼다! (CanSeePlayer와 같은 방식으로)
        // [X] => 클라에서 검사하기 보다 (클라에서 검사하면 통신 작업이 더 까다로운데다가 비정확할 수 있고 비효율적임) 서버에서 판단해서 각 클라들에게 알려주자

        bool result = zom.HeardFootSound;

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
#pragma once

#include "Sequence.h"


class Seq_NotHasLastKnownPlayerLocation : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (NotHasLastKnownPlayerLocation Decorator) 호출" << endl;
#endif

        d_result = !zom.KnewPlayerLocation;

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 플레이어의 이전 위치 정보를 가지고 있는가?: " << boolalpha << !d_result << endl;
#endif

        //if (!d_result) {
        //    cout << "좀비가 가진 플레이어 이전 위치: ( " << zom.PrevTargetLocation[0][0][0] << ", " << zom.PrevTargetLocation[0][0][1] << ", " << zom.PrevTargetLocation[0][0][2] << " )" << endl;
        //    cout << "근데, 사실상 이 코드는 실행되면 안됨!!!!!!!!! [ERROR]" << endl;
        //    // 만약, Detect Selector 를 병렬적으로 작동시키면 해당 에러 로그 필요 X
        //}

#ifdef ENABLE_BT_NODE_LOG
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (NotHasLastKnownPlayerLocation Decorator) 결과: \"" << boolalpha << d_result << "\"" << endl;
        cout << endl;
#endif

        if (d_result == true)
            NotHasLastKnownPlayerLocation(zom);

        return d_result;
    }

    bool NotHasLastKnownPlayerLocation(Zombie& zom) {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {NotHasLastKnownPlayerLocation} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            d_result = child->NotHasLastKnownPlayerLocation(zom);
        }

        if (d_result == false) {
            cout << "\"Sequence NotHasLastKnownPlayerLocation [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
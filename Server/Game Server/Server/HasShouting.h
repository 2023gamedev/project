#pragma once

#include "Sequence.h"


class Seq_HasShouting : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 [HasShouting Task] 호출" << endl;
#endif

        result = zom.HeardShouting;

        if (result == true) {
            zom.SetTargetLocation(Zombie::TARGET::SHOUTING);
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 샤우팅 소리 정보를 가지고 있는가?: " << boolalpha << result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [HasShouting Task] 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
#endif

        return result;
    }

};
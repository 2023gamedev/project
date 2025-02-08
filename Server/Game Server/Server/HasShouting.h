#pragma once

#include "Sequence.h"


class Seq_HasShouting : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (HasShouting Decorator) 호출" << endl;
#endif

        result = zom.HeardShouting;

        if (result == true) {
            zom.SetTargetLocation(Zombie::TARGET::SHOUTING);
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 샤우팅 소리 정보를 가지고 있는가?: " << boolalpha << result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (HasShouting Decorator) 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
#endif

        if (result == true)
            HasShouting(zom);

        return result;
    }

    bool HasShouting(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HasShouting} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            result = child->HasShouting(zom);
        }

        if (result == false) {
            cout << "\"Sequence HasShouting [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

};
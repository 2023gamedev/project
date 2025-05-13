#pragma once

#include "Sequence.h"


class Seq_HasShouting : public Sequence {
public:
    Seq_HasShouting() { t_name = "HasShouting"; }


    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (HasShouting Decorator) 호출" << endl;
#endif

        d_result = zom.HeardShouting;

        if (d_result == true) {
            zom.SetTargetLocation(Zombie::TARGET::SHOUTING);
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 샤우팅 소리 정보를 가지고 있는가?: " << boolalpha << d_result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (HasShouting Decorator) 결과: \"" << boolalpha << d_result << "\"" << endl;
        cout << endl;
#endif

        if (d_result == true)
            HasShouting(zom);

        return d_result;
    }

    bool HasShouting(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HasShouting} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            d_result = child->HasShouting(zom);

            if (d_result == false) {
                cout << "\"Sequence HasShouting [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
                cout << endl;

                return d_result;
            }
        }

        return d_result;
    }

};
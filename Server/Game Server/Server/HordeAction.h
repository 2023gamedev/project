#pragma once

#include "Sequence.h"


class Seq_HordeAction : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (HordeAction Decorator) 호출" << endl;
#endif

        result = zom.HeardHordeSound;

        if (result == true) {
            zom.SetTargetLocation(Zombie::TARGET::HORDESOUND); 
        }

//#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 주위 다른 좀비의 소리 정보를 가지고 있는가?: " << boolalpha << result << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (HordeSound Decorator) 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
//#endif

        if (result == true)
            HordeAction(zom);

        return result;
    }

    bool HordeAction(Zombie& zom) override {
//#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HordeAction} 호출" << endl;
        cout << endl;
//#endif

        for (const auto& child : seq_children) {
            result = child->HordeAction(zom);
        }

        if (result == false) {
            cout << "\"Sequence HordeAction [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

};
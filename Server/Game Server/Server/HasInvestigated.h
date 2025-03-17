#pragma once

#include "Sequence.h"


class Seq_HasInvestigated : public Sequence {
public:
    Seq_HasInvestigated() { t_name = "HasInvestigated"; }


    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (HasInvestigated Decorator) 호출" << endl;
#endif

        d_result = zom.KnewPlayerLocation;

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 플레이어의 이전 위치 정보를 가지고 있는가?: " << boolalpha << d_result << endl;
        if (d_result)
            cout << "좀비가 가진 플레이어 이전 위치: ( " << zom.LastKnownTargetLocation[0][0][0] << ", " << zom.LastKnownTargetLocation[0][0][1] << ", " << zom.LastKnownTargetLocation[0][0][2] << " )" << endl;
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (HasInvestigated Decorator) 결과: \"" << boolalpha << d_result << "\"" << endl;
        cout << endl;
#endif

        if (d_result == true)
            HasInvestigated(zom);

        return d_result;
    }

    bool HasInvestigated(Zombie& zom) {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HasInvestigated} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            d_result = child->HasInvestigated(zom);
        }

        if (d_result == false) {
            cout << "\"Sequence HasInvestigated [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
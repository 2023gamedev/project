﻿#pragma once

#include "Sequence.h"


class Seq_HasFootSound : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (HasFootSound Decorator) 호출" << endl;
#endif

        d_result = zom.FootSound_Update_Check();

#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 새로운 플레이어 발소리 정보를 포착했는가?: " << boolalpha << result << endl;
#endif

        if (d_result == true) // 새로운 발소리를 들었을 경우에만 실행해서 가장 가까운 플레이어의 발소리를 목표지점으로
            zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        if (zom.HeardFootSound == true) { // FootSound_Update_Check 결과 상관없이 이미 근처 발소리를 들은 경우에는(이전에 들은 발소리를 아직 기억하는 경우) -> 계속 실행
#ifdef ENABLE_BT_NODE_LOG
            if(result == false)
                cout << "좀비 플레이어 발소리 정보를 이미 가지고 있는가?: true" << endl;
#endif

            d_result = true;
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (HasFootSound Decorator) 결과: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
#endif

        if (d_result == true)
            HasFootSound(zom);

        return d_result;
    }

    bool HasFootSound(Zombie& zom) {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HasFootSound} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            d_result = child->HasFootSound(zom);
        }

        if (d_result == false) {
            cout << "\"Sequence HasFootSound [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
#pragma once

#include <iostream>

#include "Task.h"
#include "iocpServerClass.h"

using std::cout;
using std::endl;


class T_MoveTo : public Task {
public:

    bool CanNotAttack(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{CanNotAttack}의 [MoveTo Task] 호출" << endl;
#endif

        // 플레이어를 보고 있으니, 계속 따라오게 해야되니까 => 매 틱마다 위치 갱신
        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());
        
        //result = zom.MoveTo();
        result = true;
        return result;
    }

    bool HasShouting(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{HasShouting}의 [MoveTo Task] 호출" << endl;
#endif

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        result = true;
        return result;
    }

    bool HasFootSound(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{HasFootSound}의 [MoveTo Task] 호출" << endl;
#endif

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        result = true;
        return result;
    }

    bool HasInvestigated(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{HasInvestigated}의 [MoveTo Task] 호출" << endl;
#endif

        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        result = true;
        return result;
    }

    bool NotHasLastKnownPlayerLocation(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{NotHasLastKnownPlayerLocation}의 [MoveTo Task] 호출" << endl;
#endif

        zom.SetTargetLocation(Zombie::TARGET::PATROL);

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        result = true;
        return result;
    }

};
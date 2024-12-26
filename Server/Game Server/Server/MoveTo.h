#pragma once

#include <iostream>

#include "Task.h"
#include "iocpServerClass.h"

using std::cout;
using std::endl;


class TMoveTo : public Task {
public:

    string CanNotAttack(Zombie& zom) const override {
        //cout << "{CanNotAttack}의 [MoveTo Task] 호출" << endl;

        // 플레이어를 보고 있으니, 계속 따라오게 해야되니까 => 매 틱마다 위치 갱신
        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());
        
        //bool result = zom.MoveTo(); 
        //if (result)
        //    return "MoveTo-Succeed";
        //else
        //    return "Fail";

        return "MoveTo-Succeed";
    }

    string HasShouting(Zombie& zom) const override {
        //cout << "{HasShouting}의 [MoveTo Task] 호출" << endl;

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        return "MoveTo-Succeed";
    }

    string HasFootSound(Zombie& zom) const override {
        //cout << "{HasFootSound}의 [MoveTo Task] 호출" << endl;

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        return "MoveTo-Succeed";
    }

    string HasInvestigated(Zombie& zom) const override {
        //cout << "{HasInvestigated}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        return "MoveTo-Succeed";
    }

    string NotHasLastKnownPlayerLocation(Zombie& zom) const override {
        //cout << "{NotHasLastKnownPlayerLocation}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::PATROL);

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        return "MoveTo-Succeed";
    }

    //사실상 더미 함수들
    string Detect(Zombie& zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    string CanAttack(Zombie& zom) const override { return "Fail"; };
    //string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    //string HasShouting(Zombie& zom) const override { return "Fail"; };
    //string HasFootSound(Zombie& zom) const override { return "Fail"; };
    //string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    //string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};
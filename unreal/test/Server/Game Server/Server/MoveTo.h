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

        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_INTERVAL);

        zom.SendPath();
        
        //bool result = zom.MoveTo(); 
        //if (result)
        //    return "MoveTo-Succeed";
        //else
        //    return "Fail";

        return "MoveTo-Succeed";
    }

    string HasShouting(Zombie& zom) const override {
        //cout << "{HasShouting}의 [MoveTo Task] 호출" << endl;

        //zom.SetTargetLocation(Zombie::TARGET::SHOUTING);  // ==========>> 얘네는 해당 소리를 듣는 순간, 그니까 그때 그순간의 타겟의 위치로 타겟을 지정해야해서 
                                                            //  ProcessPacket에서 받을때 초기화 해줘야 할듯하다

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_INTERVAL);

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string HasFootSound(Zombie& zom) const override {
        //cout << "{HasFootSound}의 [MoveTo Task] 호출" << endl;

        //zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);     // ==========>> 얘네는 해당 소리를 듣는 순간, 그니까 그때 그순간의 타겟의 위치로 타겟을 지정해야해서 
                                                                //  ProcessPacket에서 받을때 초기화 해줘야 할듯하다

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_INTERVAL);

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string HasInvestigated(Zombie& zom) const override {
        //cout << "{HasInvestigated}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_INTERVAL);

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string NotHasLastKnownPlayerLocation(Zombie& zom) const override {
        //cout << "{NotHasLastKnownPlayerLocation}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::PATROL);

        zom.MoveTo(IOCP_CORE::BT_INTERVAL);

        zom.SendPath();

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
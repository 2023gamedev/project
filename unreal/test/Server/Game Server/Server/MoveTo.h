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

        // 플레이어를 보고 있으니, 계속 따라오게 해야되니까 
        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.UpdatePath();

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

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
                                                            //  여기다가 놔두면 계속 위치 갱신됨 -> ProcessPacket에서 받을때 초기화 해줘야 할듯하다 

        //zom.UpdatePath();                 // 얘도 맨밑에 Patrol 처럼 타겟 위치 변화 없을 테니까 매번 하지 말고 SetTargetLocation에서 한번만 작업해 주자

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string HasFootSound(Zombie& zom) const override {
        //cout << "{HasFootSound}의 [MoveTo Task] 호출" << endl;

        //zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);     // ==========>> 얘네는 해당 소리를 듣는 순간, 그니까 그때 그순간의 타겟의 위치로 타겟을 지정해야해서 
                                                                // 여기다가 놔두면 계속 위치 갱신됨 -> ProcessPacket에서 받을때 초기화 해줘야 할듯하다

        //zom.UpdatePath();                 // 얘도 맨밑에 Patrol 처럼 타겟 위치 변화 없을 테니까 매번 하지 말고 SetTargetLocation에서 한번만 작업해 주자

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string HasInvestigated(Zombie& zom) const override {
        //cout << "{HasInvestigated}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        //zom.UpdatePath();                 // 얘도 맨밑에 Patrol 처럼 타겟 위치 변화 없을 테니까 매번 하지 말고 SetTargetLocation에서 한번만 작업해 주자

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

        zom.SendPath();

        return "MoveTo-Succeed";
    }

    string NotHasLastKnownPlayerLocation(Zombie& zom) const override {
        //cout << "{NotHasLastKnownPlayerLocation}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::PATROL);

        //zom.UpdatePath();         // 패트롤 지점은 위치 변화가 없기에 한번만 작업해주면 됨 => SetTargetLocation->RandPatrol 에서 이미 작업해줌
                                    // A* 내부코드로 이미 목표지점의 위치 변화가 없으면 A* 안돌리는 코드가 있지만..

        zom.MoveTo(IOCP_CORE::BT_deltaTime.count());

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
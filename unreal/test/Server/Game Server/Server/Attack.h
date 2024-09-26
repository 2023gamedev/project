#pragma once

#include <iostream>

#include "Task.h"

using std::cout;
using std::endl;


class TAttack : public Task {
public:

    string CanAttack(Zombie& zom) const override {
        //cout << "{CanAttack}의 [Attack Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::PLAYER);      //좀비 공격 범위에 플레이어 있을 때도 플레이어의 최신 위치를 갱신해주는게 맞는거 같아서

        if (zom.IsAttacking == false)
            zom.Attack();
        else
            zom.Wait();
        
        //bool result = zom.Attack(); 
        //if (result)
        //    return "Attack-Succeed";
        //else
        //    return "Fail";

        return "Attack-Succeed";
    }

    //사실상 더미 함수들
    string Detect(Zombie& zom) const override { return "Fail"; };
    string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    //string CanAttack(Zombie& zom) const override { return "Fail"; };
    string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    string HasShouting(Zombie& zom) const override { return "Fail"; };
    string HasFootSound(Zombie& zom) const override { return "Fail"; };
    string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};
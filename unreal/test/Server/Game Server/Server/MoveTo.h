#pragma once

#include <iostream>

#include "Task.h"

using std::cout;
using std::endl;


class TMoveTo : public Task {
public:

    string CanNotAttack(Zombie& zom) const override {
        //cout << "{CanNotAttack}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::PLAYER);

        zom.MoveTo();

        zom.Wait();
        
        //bool result = zom.MoveTo(); 
        //if (result)
        //    return "MoveTo-Succeed";
        //else
        //    return "Fail";

        return "MoveTo-Succeed";
    }

    string HasShouting(Zombie& zom) const override {
        //cout << "{HasShouting}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::SHOUTING);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string HasFootSound(Zombie& zom) const override {
        //cout << "{HasFootSound}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::FOOTSOUND);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string HasInvestigated(Zombie& zom) const override {
        //cout << "{HasInvestigated}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::INVESTIGATED);

        zom.MoveTo();

        zom.Wait();

        return "MoveTo-Succeed";
    }

    string NotHasLastKnownPlayerLocation(Zombie& zom) const override {
        //cout << "{NotHasLastKnownPlayerLocation}의 [MoveTo Task] 호출" << endl;

        zom.SetTargetLocation(Zombie::TARGET::ORIGIN);      //원래 자리로 돌아가기 보다는 랜덤한 근처 장소로 이동하게 만들어서 배회하게끔 만들면 좋을 듯

        zom.MoveTo();

        zom.Wait();

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
#pragma once

#include <iostream>


#include "Zombie.h"

using std::cout;
using std::endl;
using std::string;


class Zombie;

class Task {
public:
    string t_name = "";
    //결과값 저장
    bool result = false;


    //다음으로 행동할 Task 감지 [Selector-Service]
    virtual bool Detect(Zombie& zom) { return false; };

    //플레이어를 보았는가 감지 [Selector-Decorator]
    virtual bool CanSeePlayer(Zombie& zom) { return false; };

    //공격 범위 내 검사 [Sequence-Decorator]
    virtual bool CanAttack(Zombie& zom) { return false; };

    //CanAttack()의 역조건 [Sequence-Decorator]
    virtual bool CanNotAttack(Zombie& zom) { return false; };

    //샤우팅 반경 내 검사 [Sequence-Decorator]
    virtual bool HasShouting(Zombie& zom) { return false; };

    //발소리 반경 내 검사 [Sequence-Decorator]
    virtual bool HasFootSound(Zombie& zom) { return false; };

    //주위에 다른 좀비들이 소리를 내었는지 검사 [Sequence-Decorator]
    virtual bool HordeAction(Zombie& zom) { return false; };

    //플레이어의 마지막 위치를 기억하는지 검사 [Sequence-Decorator]
    virtual bool HasInvestigated(Zombie& zom) { return false; };

    //HasInvestigated()의 역조건 [Sequence-Decorator]
    virtual bool NotHasLastKnownPlayerLocation(Zombie& zom) { return false; };


    Task() = default;

    virtual ~Task() = default;
};
#pragma once

#include <iostream>
#include <string>

#include "Zombie.h"

using namespace std;


class Zombie;

class Task {
public:
    //다음으로 행동할 Task 감지 [Selector-Service]
    virtual string Detect(Zombie& zom) const = 0;

    //플레이어를 보았는가 감지 [Selector-Decorator]
    virtual string CanSeePlayer(Zombie& zom) const = 0;

    //공격 범위 내 검사 [Sequence-Decorator]
    virtual string CanAttack(Zombie& zom) const = 0;

    //CanAttack()의 역조건 [Sequence-Decorator]
    virtual string CanNotAttack(Zombie& zom) const = 0;

    //샤우팅 반경 내 검사 [Sequence-Decorator]
    virtual string HasShouting(Zombie& zom) const = 0;

    //발소리 반경 내 검사 [Sequence-Decorator]
    virtual string HasFootSound(Zombie& zom) const = 0;

    //플레이어의 마지막 위치를 기억하는지 검사 [Sequence-Decorator]
    virtual string HasInvestigated(Zombie& zom) const = 0;

    //HasInvestigated()의 역조건 [Sequence-Decorator]
    virtual string NotHasLastKnownPlayerLocation(Zombie& zom) const = 0;


    Task() = default;

    virtual ~Task() = default;
};
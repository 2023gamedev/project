#pragma once

#include "Zombie.h"

class Zombie;

class Task {
public:
    //샤우팅 [Task]
//virtual bool Shouting() const = 0;     
// 
// 
// 
// 
// 
// 
// 
    //다음으로 행동할 Task 감지 [Selector-Service]
    virtual bool Detect(Zombie zom) const = 0;

    //플레이어를 보았는가 감지 [Selector-Decorator]
    virtual bool CanSeePlayer(Zombie zom) const = 0;

    //공격 범위 내 검사 [Sequence-Decorator]
    virtual bool CanAttack(Zombie zom) const = 0;

    //CanAttack()의 역조건 [Sequence-Decorator]
    virtual bool CanNotAttack(Zombie zom) const = 0;

    //샤우팅 반경 내 검사 [Sequence-Decorator]
    virtual bool HasShouting(Zombie zom) const = 0;

    //발소리 반경 내 검사 [Sequence-Decorator]
    virtual bool HasFootSound(Zombie zom) const = 0;

    //플레이어의 마지막 위치를 기억하는지 검사 [Sequence-Decorator]
    virtual bool HasInvestigated(Zombie zom) const = 0;

    //HasInvestigated()의 역조건 [Sequence-Decorator]
    virtual bool NotHasLastKnownPlayerLocation(Zombie zom) const = 0;


    Task() = default;

    virtual ~Task() = default;
};
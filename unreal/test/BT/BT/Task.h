#pragma once

class Task {
public:
    //다음으로 행동할 Task 감지 [Selector-Service]
    virtual bool Detect() const = 0;    

    //플레이어를 보았는가 감지 [Selector-Decorator]
    virtual bool CanSeePlayer() const = 0;  

    //공격 [Task]
    virtual bool Attack() const = 0;    
    
    //이동 [Task]
    virtual bool MoveTo() const = 0;    

    //대기 [Task]
    virtual bool Wait() const = 0;      

    //샤우팅 [Task]
    virtual bool Shouting() const = 0;     

    //블랙보드(발견 플레이어 위치) 초기화 [Task]
    virtual bool ClearBlackboardValue() const = 0;  

    //공격 범위 내 검사 [Sequence-Decorator]
    virtual bool CanAttack() const = 0;   

    //CanAttack()의 역조건 [Sequence-Decorator]
    virtual bool CanNotAttack() const = 0;
    
    //CanAttack()의 역조건 -> 행동 초기화 [Sequence-Service]
    virtual bool ServiceCanNotAttack() const = 0;    

    //플레이어의 마지막 위치를 기억하는지 검사 [Sequence-Decorator]
    virtual bool HasInvestigated() const = 0;     

    //발소리 반경 내 검사 [Sequence-Decorator]
    virtual bool HasFootSound() const = 0;      

    //샤우팅 반경 내 검사 [Sequence-Decorator]
    virtual bool HasShouting() const = 0;

    //HasInvestigated()의 역조건 [Sequence-Decorator]
    virtual bool NotHasLastKnownPlayerLoaction() const = 0;


    virtual ~Task() = default;
};
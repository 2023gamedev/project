#pragma once

class Task {
public:
    virtual bool Attack() const = 0;    //공격

    virtual bool MoveTo() const = 0;    //이동

    virtual bool Wait() const = 0;      //대기

    virtual bool ClearBlackboardValue() const = 0;  //발견 플레이어 위치 초기화

    virtual ~Task() = default;
};
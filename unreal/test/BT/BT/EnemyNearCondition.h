#pragma once

#include "Task.h"
#include "Enemies.h"

class EnemyNearCondition : public Task {
public:
    // 생성자
    EnemyNearCondition(float targetDistance = 10.0f) : TargetDistance(targetDistance) {}

    // Run 메서드 구현
    bool Run() const override {
        // 정해진 거리 이내에 적이 있다면 true 반환
        return Enemies::DistanceToClosestEnemy <= TargetDistance;
    }

private:
    float TargetDistance;
};
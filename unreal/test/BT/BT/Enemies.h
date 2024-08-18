#pragma once

class Enemies {
public:
    static float DistanceToClosestEnemy; // 예시로 전역 변수 사용
};

// 초기화 (실제 값은 게임 로직에 맞게 설정)
float Enemies::DistanceToClosestEnemy = 20.0f;
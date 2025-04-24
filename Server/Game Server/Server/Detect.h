#pragma once

#include "Selector.h"


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "=== Selector <Detect> 호출" << endl;
        cout << endl;
#endif

        d_result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->Detect(zom);  // 다형성 이용 (함수 오버라이딩)

            d_results[child->t_name] = task_result; // Selector의 데코레이터 결과값들 저장 [데코레이터 이름-결과값]

            if (d_result == false)
                d_result = task_result; // 어느 하나라도 결과값이 true이면 true

            if (d_result == true) // <= 나중에 병렬로 돌리려면 지워야함
                break;  // SetTargetLocation이 문제라 병렬로 돌리면 현재로써는 안 됨...;; (TargetLocation 자꾸 덮어씌우기함) -> (깊이 우선 탐색 => 너비 우선 탐색) 바꿔야함!
        }

        if (d_result == false) {  // selector의 모든 decorator가 실패 할 경우(그럴 일은 없어야 하지만..)
            //cout << "\"Selector Detect [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            //cout << endl;
            // ********************************************************* 여기 나중에 주석 풀기 (좀비 BT 수정 다 하고) ******************************************************************
        }

        return d_result;
    }

};
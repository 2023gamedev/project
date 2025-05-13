#pragma once

#include "Task.h"


class T_LookAround : public Task {
public:


	// 이후에 실제로 Task 형식으로 사용해야 할 경우 구현 필요 (지금은 그냥 함수 형태로 불러서 사용중 - 그래서 중복 코드들 존재;;[ReachFinalDestination에서])
	


//    bool CanAttack(Zombie& zom) override {
//#ifdef ENABLE_BT_NODE_LOG
//        cout << "{CanAttack}의 [Attack Task] 호출" << endl;
//#endif
//
//        zom.SetTargetLocation(Zombie::TARGET::PLAYER);      //좀비 공격 범위에 플레이어 있을 때도 플레이어의 최신 위치를 갱신해주는게 맞는거 같아서 (근데 공격 시작하면 플레이어 시야 검사 X - 즉, 공격하기 직전 위치를 기억)
//
//        // BT 시작시에 이미 zom.HaveToWait을 이용해서 Wait으로 들어가게 구현했지만 혹시 몰라서;;
//        if (zom.IsAttacking == false)
//            zom.Attack(zom.roomid);
//        else
//            zom.Wait();
//
//        //bool d_result = zom.Attack(); 
//        d_result = true;
//        return d_result;
//    }

};
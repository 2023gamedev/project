#pragma once

#include "Task.h"


class T_Attack : public Task {
public:

    bool CanAttack(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "{CanAttack}의 [Attack Task] 호출" << endl;
#endif

        zom.SetTargetLocation(Zombie::TARGET::PLAYER);      //좀비 공격 범위에 플레이어 있을 때도 플레이어의 최신 위치를 갱신해주는게 맞는거 같아서
 
        // BT 시작시에 이미 zom.HaveToWait을 이용해서 Wait으로 들어가게 구현했지만 혹시 몰라서;;
        if (zom.IsAttacking == false)
            zom.Attack(zom.roomid);
        else
            zom.Wait();

        //bool result = zom.Attack(); 
        result = true;
        return result;
    }

};
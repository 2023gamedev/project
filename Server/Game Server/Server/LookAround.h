#pragma once

#include "Task.h"


class T_LookAround : public Task {
public:


	// ���Ŀ� ������ Task �������� ����ؾ� �� ��� ���� �ʿ� (������ �׳� �Լ� ���·� �ҷ��� ����� - �׷��� �ߺ� �ڵ�� ����;;[ReachFinalDestination����])
	


//    bool CanAttack(Zombie& zom) override {
//#ifdef ENABLE_BT_NODE_LOG
//        cout << "{CanAttack}�� [Attack Task] ȣ��" << endl;
//#endif
//
//        zom.SetTargetLocation(Zombie::TARGET::PLAYER);      //���� ���� ������ �÷��̾� ���� ���� �÷��̾��� �ֽ� ��ġ�� �������ִ°� �´°� ���Ƽ� (�ٵ� ���� �����ϸ� �÷��̾� �þ� �˻� X - ��, �����ϱ� ���� ��ġ�� ���)
//
//        // BT ���۽ÿ� �̹� zom.HaveToWait�� �̿��ؼ� Wait���� ���� ���������� Ȥ�� ����;;
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
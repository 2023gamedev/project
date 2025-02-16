#pragma once

#include "Selector.h"


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Selector <Detect> ȣ��" << endl;
        cout << endl;
#endif

        d_result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->Detect(zom);  // ������ �̿� (�Լ� �������̵�)

            if (d_result == false)
                d_result = task_result;

            if (d_result == true)
                break;  // SetTargetLocation�� ������ ���ķ� ������ ����ν�� �� ��...;; (TargetLocation �ڲ� ��������)
        }

        if (d_result == false) {  // selector�� ��� task�� ���� �� ���(�׷� ���� ����� ������..)
            cout << "\"Selector Detect [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
#pragma once

#include "Selector.h"


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect> ȣ��" << endl;
        cout << endl;
#endif

        result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->Detect(zom);  // ������ �̿� (�Լ� �������̵�)

            if (result == false)
                result = task_result;
        }

        if (result == false) {  // selector�� ��� task�� ���� �� ���(�׷� ���� ����� ������..)
            cout << "\"Selector Detect [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

};
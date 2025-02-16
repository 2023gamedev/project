#pragma once

#include "Sequence.h"


class Seq_HordeAction : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>�� (HordeAction Decorator) ȣ��" << endl;
#endif

        d_result = zom.HeardHordeSound;

        if (d_result == true) {
            zom.SetTargetLocation(Zombie::TARGET::HORDESOUND); 
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "���� ���� �ٸ� ������ �Ҹ� ������ ������ �ִ°�?: " << boolalpha << d_result << endl;
        cout << "����, ���� \'#" << zom.ZombieData.zombieID << "\' �� <Detect>�� (HordeSound Decorator) ���: \"" << boolalpha << d_result << "\"" << endl;
        cout << endl;
#endif

        if (d_result == true)
            HordeAction(zom);

        return d_result;
    }

    bool HordeAction(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HordeAction} ȣ��" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            d_result = child->HordeAction(zom);
        }

        if (d_result == false) {
            cout << "\"Sequence HordeAction [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};
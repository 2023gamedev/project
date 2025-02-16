#pragma once

#include "Sequence.h"


class Seq_HordeAction : public Sequence {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>�� (HordeAction Decorator) ȣ��" << endl;
#endif

        result = zom.HeardHordeSound;

        if (result == true) {
            zom.SetTargetLocation(Zombie::TARGET::HORDESOUND); 
        }

//#ifdef ENABLE_BT_NODE_LOG
        cout << "���� ���� �ٸ� ������ �Ҹ� ������ ������ �ִ°�?: " << boolalpha << result << endl;
        cout << "����, ���� \'#" << zom.ZombieData.zombieID << "\' �� <Detect>�� (HordeSound Decorator) ���: \"" << boolalpha << result << "\"" << endl;
        cout << endl;
//#endif

        if (result == true)
            HordeAction(zom);

        return result;
    }

    bool HordeAction(Zombie& zom) override {
//#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {HordeAction} ȣ��" << endl;
        cout << endl;
//#endif

        for (const auto& child : seq_children) {
            result = child->HordeAction(zom);
        }

        if (result == false) {
            cout << "\"Sequence HordeAction [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

};
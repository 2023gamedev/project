#pragma once

#include <iostream>

#include "Selector.h"
#include "iocpServerClass.h"

using std::cout;
using std::endl;


class Sel_Detect : public Selector {
public:

    bool Detect(Zombie& zom) override {
        //cout << "<Detect> ȣ��" << endl;

        result = true;

        return result;
    }

};
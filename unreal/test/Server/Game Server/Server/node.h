#pragma once
#include "Common.h"

struct Nvector {
    float x, y, z;
};

class Gnode
{
    Gnode();
    ~Gnode();

public:
    vector<Nvector> LoadNodesFromFile();

};

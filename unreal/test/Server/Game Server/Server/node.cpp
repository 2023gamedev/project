#include "node.h"

Gnode::Gnode()
{
}

Gnode::~Gnode()
{
}

vector<Nvector> Gnode::LoadNodesFromFile()
{
    vector<Nvector> Nodes;
    ifstream File("Nodes.txt");
    string Line;

    if (File.is_open())
    {
        while (getline(File, Line))
        {
            Nvector Vec;
            sscanf_s(Line.c_str(), "%f,%f,%f", &Vec.x, &Vec.y, &Vec.z);
            Nodes.push_back(Vec);
        }
        File.close();
    }
    else
    {
        cerr << "Failed to open the Nodes.txt" << endl;
    }

    return Nodes;
}
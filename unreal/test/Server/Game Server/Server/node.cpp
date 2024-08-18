#include "node.h"

vector<Vector> node::LoadNodesFromFile()
{
    vector<Vector> Nodes;
    ifstream File("Nodes.txt");
    string Line;

    if (File.is_open())
    {
        while (getline(File, Line))
        {
            Vector Vec;
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
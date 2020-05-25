#include "simulator.hpp"

void read_file()
{
  ifstream file("spice.txt");
  string line, device;
  int node1, node2;
  double value;

  node ref;
  ref.index = 0;

  circuit A;
  A.nodes.push_back(ref);
  while(getline(file, line))
  {
      stringstream input(line);
      input >> device >> node1 >> node2 >> value;
      if(device.find('R') > -1)
      {
        R r1(device, value);
        for(int i=0; i<A.nodes.size(); i++)
        {
          if( (node1 || node2) == A.nodes[i].index)
            {
              ref.connected_components.push_back(r1);
            }
            else if(node1 != A.nodes[i].index)
            {
              node new_node(node1);
              new_node.connected_components.push_back(r1);
              A.nodes.push_back(new_node);
            }
            else if(node2 != A.nodes[i].index)
            {
              node new_node(node2);
              new_node.connected_components.push_back(r1);
              A.nodes.push_back(new_node);
            }
            else
            {

            }
        }
      }
      else if(device.find('C') > -1)
      {
        C c1(device, value);
        if( (node1 || node2) == ref.index)
        {
          ref.connected_components.push_back(c1);
        }
      }
      else if(device.find('L') > -1)
      {
        L l1(device, value);
        if( (node1 || node2) == ref.index)
        {
          ref.connected_components.push_back(l1);
        }
      }
  }
}

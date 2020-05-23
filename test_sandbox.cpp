// This is a compilable test-sandbox.
// New functions can be compiled and tested on the fly here.
#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

int main()
{
    //cout << suffix_parser("") << endl;
    R R1(10.0);
    R R2(20.0);
    node node1;
    node1.node_index = "nodenum1";
    node1.connected_components = {R1,R2};
    // cout << parse_netlist_line(sim, ".tran 0 10ms 0 1us");
    double output;
    output = calculate_conductance_between_nodes(node1,node1);
    cout << output << endl;
}

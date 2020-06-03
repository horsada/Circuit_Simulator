// This is a compilable test-sandbox.
// New functions can be compiled and tested here
#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

int main() {
    //cout << suffix_parser("") << endl;
    network_simulation sim;
    // R* r1 = new R1("R1", 10.0);
    // R R1("R1", 10.0);
    // R R2("R1", 20.0);
    // cout << R1.read_value();
    //node1.node_index = "nodenum1";
    //node1.connected_components = {R1,R2};
    cout << "return code: "<< parse_netlist_line(sim, ".tran 0 10ms 0 1Megs") << endl;
    cout << sim.stop_time << endl;
    cout << sim.timestep << endl;
    // double output;
    // output = calculate_conductance_between_nodes(node1,node1);
    //cout << output << endl;
}

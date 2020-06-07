// This is a compilable test-sandbox.
// New functions can be compiled and tested here
#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

int main() {
    //cout << suffix_parser("") << endl;
    network_simulation sim;
    // R R1("R1", 10.0);
    // R R2("R1", 20.0);
    // cout << R1.read_value();
    //node1.node_index = "nodenum1";
    //node1.connected_components = {R1,R2};

    // Testing netlist_parser for resistors
    parse_netlist_line(sim, ".tran 0 10ms 0 1Megs");
    parse_netlist_line(sim, "L1 N001 0 1Meg");
    parse_netlist_line(sim, "L1 N001 0 1");
    parse_netlist_line(sim, "C1 N002 0 1.5k");
    parse_netlist_line(sim, "R5 N003 0 1Meg");
    parse_netlist_line(sim, "I1 N003 0 SINE(2 1 1000)");
    parse_netlist_line(sim, "V5 N003 0 1.2k");

    cout << sim.stop_time << endl;
    cout << sim.timestep << endl;

    cout << "no. of nodes: " << sim.network_nodes.size() << endl;
    for(auto const& value: sim.network_nodes) {
     cout << "Node found: " << value.index << endl;
		 cout << "connected_components=" << value.connected_components.size() << endl;
    }
		
    cout << "no. of components: " << sim.network_components.size() << endl;
    for(auto const& value: sim.network_components) {

     cout << "Component found: " << value.component_name << endl;
     cout << "type=" << value.component_name[0] << endl;
     cout << "value=" << value.read_value()[0] << endl;
     if(value.component_name[0]=='V') {
       cout << "dc_offset=" << value.read_value()[0] << endl;
       cout << "amplitude=" << value.read_value()[1] << endl;
       cout << "frequency=" << value.read_value()[2] << endl;
     }


    }

}

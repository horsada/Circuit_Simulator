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
    parse_netlist_line(sim, "R1 N001 0 1.7k");
    parse_netlist_line(sim, "R3 N002 0 1.43k");
    parse_netlist_line(sim, "R1 N003 0 10");

    // cout << sim.stop_time << endl;
    // cout << sim.timestep << endl;

    double cbn = calculate_conductance_between_nodes(sim.network_nodes[0], sim.network_nodes[1]);

    vector<node> nds = create_v_matrix(sim);
    cout << nds.size();
    for(auto nd: nds) {
      cout << nd.index;
    }
    // MatrixXd x = create_G_matrix(sim);
    // cout << "matrix:" << endl << x << endl;

}

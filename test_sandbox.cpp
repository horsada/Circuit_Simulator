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
    parse_netlist_line(sim, ".tran 0 10 0 1ms");
    parse_netlist_line(sim, "I1 N003 0 10");
    parse_netlist_line(sim, "I1 0 N003 2");
    parse_netlist_line(sim, "R1 N001 N003 1k");
    parse_netlist_line(sim, "R2 N002 N003 2k");
    parse_netlist_line(sim, "V2 N005 N006 5");
    parse_netlist_line(sim, "V2 N008 N009 5");

    // cout << sim.stop_time << endl;
    // cout << sim.timestep << endl;

    // vector<component> components, node reference_node
    // vector<pair<node, node>> x = supernode_separation(sim.network_components, sim.network_nodes[0]);
    // cout << "idx1.1=" << x[0].first.index << endl;
    // cout << "idx1.2=" << x[0].second.index << endl;
    // cout << "idx2.1=" << x[1].first.index << endl;
    // cout << "idx2.2=" << x[1].second.index << endl;
    // cout << "size=" << x.size() << endl;

    double sigma_I = sum_known_currents(sim.network_nodes[0], 1.0);
    cout << "sigma_I at node N" << sim.network_nodes[0].index << " equals: " << sigma_I << endl;
    // vector<node> nds = create_v_matrix(sim);
    // cout << nds.size();
    // for(auto nd: nds) {
    //   cout << nd.index;
    // }
    // MatrixXd x = create_G_matrix(sim);
    // cout << "matrix:" << endl << x << endl;

}

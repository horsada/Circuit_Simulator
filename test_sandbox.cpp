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
    parse_netlist_line(sim, ".tran 0 10s 0 1ms");
    parse_netlist_line(sim, "V1 N001 0 5");
    parse_netlist_line(sim, "I2 N003 N004 7");
    parse_netlist_line(sim, ".end");


    cout << sim.stop_time << endl;
    cout << sim.timestep << endl;

    // vector<component> components, node reference_node
    // vector<pair<node, node>> x = supernode_separation(sim.network_components, sim.network_nodes[0]);
    // cout << "idx1.1=" << x[0].first.index << endl;
    // cout << "idx1.2=" << x[0].second.index << endl;
    // cout << "idx2.1=" << x[1].first.index << endl;
    // cout << "idx2.2=" << x[1].second.index << endl;
    // cout << "size=" << x.size() << endl;

    //double sigma_I = sum_known_currents(sim.network_nodes[0], 1.0);
    //cout << "sigma_I at node N" << sim.network_nodes[0].index << " equals: " << sigma_I << endl;
    // vector<node> nds = create_v_matrix(sim);
    // cout << nds.size();
    // for(auto nd: nds) {
    //   cout << nd.index;
    // }
    // MatrixXd x = create_G_matrix(sim);
    // cout << "matrix:" << endl << x << endl;

  	MatrixXd G = create_G_matrix(sim);
  	cout << "G_matrix:" << endl << G << endl << endl;

    cout << "V_matrix:" << endl;
    vector<node> V;
    V = create_v_matrix(sim);
    for(int i = 0; i < V.size(); i++){
      cout << V[i].index << endl ;
    }

  	MatrixXd I = create_i_matrix(sim, 5.0);
  	cout << endl << endl << "I_matrix:" << endl << I << endl << endl;

}


/*

index is = 0
there are 2components connected to N1
in node: 1 there is a component called: V1
in node: 1 there is a component called: R1
index is = 1
there are 2components connected to N0
in node: 0 there is a component called: V1
in node: 0 there is a component called: R3
index is = 2
there are 2components connected to N3
in node: 3 there is a component called: V2
in node: 3 there is a component called: R2
index is = 3
there are 2components connected to N4
in node: 4 there is a component called: V2
in node: 4 there is a component called: R3
index is = 4
there are 2components connected to N2
in node: 2 there is a component called: R1
in node: 2 there is a component called: R2



SND1=3
SND1 connected components: 0
SND2=4
SND2 connected components: 0

*/

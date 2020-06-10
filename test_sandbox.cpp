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
    parse_netlist_line(sim, "R1 N001 0 10");
    parse_netlist_line(sim, "R2 N001 N002 10");
    parse_netlist_line(sim, "I1 N002 0 10");
    parse_netlist_line(sim, "V2 N001 N003 10");
	parse_netlist_line(sim, "R3 N003 N002 10");
	// cout << sim.stop_time << endl;
    // cout << sim.timestep << endl;

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
	MatrixXd G;
	G = create_G_matrix(sim);
	cout << G(0,0) <<"," << G(0,1) <<"," <<G(0,2)<<endl << G(1,0)<< "," << G(1,1) << "," << G(1,2)<< endl << G(2,0) << "," << G(2,1) << "," << G(2,2) << endl;
	vector<node> V;
	V = create_v_matrix(sim);
	for(int i = 0; i < V.size(); i++){
		cout << V[i].index << endl ;
	}

}

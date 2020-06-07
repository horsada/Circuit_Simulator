#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;

using namespace Eigen;

int main(){


	network_simulation sim;

	vector<node> nodes;

	node node0(0);
	node node1(1);
	node node2(2);
	nodes.push_back(node0);
	nodes.push_back(node1);
	nodes.push_back(node2);

	sim.network_nodes = nodes;

	vector<node> matrix;

	matrix = create_v_matrix(sim);
	int a,b;
	a = matrix[0].index;
	b = matrix[1].index;
	cout<< a << " " << b << endl;
}

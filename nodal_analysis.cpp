#include "dependencies.hpp"
#include "simulator.hpp"

double get_impedance(component r){
	return r.read_value();
}

// complex<float> get_impedance(C c, float angular_fre){
// 	float interm;
// 	interm = c.read_value();
// 	return {0, -1/(angular_fre*interm)};
// }
//
// complex<float> get_impedance(L l, float angular_fre){
// 	float interm;
// 	interm = l.read_value();
// 	return {0, angular_fre*interm};
// }
//
// complex<float> get_impedance(diode d, float angular_fre){}

//diode is leftout for now. I think it can be treated as a current source with an extra switch, which is controlled by the voltage difference across the diode

double sum_conductance(vector<component> components) {
	double total_conductance;
	total_conductance = 0.0;
	for(int i = 0; i < components.size(); i++) {
		total_conductance += 1.0/get_impedance(components[i]);
	}
	return total_conductance;
}

double calculate_conductance_between_nodes(node nodeinput1, node nodeinput2)
{

	// For diagonal conductance matrix entries (G11 ,G22, G33 ...)
	if(nodeinput1.node_index == nodeinput2.node_index){
		return sum_conductance(nodeinput1.connected_components);
	}

	// For all other conductance matrix entries (G12, G21, G13, G31 ...)
	/*else if (nodeinput1.node_index != nodeinput2.node_index) {

		// To find the common intersection, the vectors need to be sorted
		sort(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end());
		sort(nodeinput2.connected_components.begin(),nodeinput2.connected_components.end());

		vector<component> common_components_between_node(nodeinput1.connected_components.size()+nodeinput2.connected_components.size());
		vector<component>::iterator it;
		it = set_intersection(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end(),nodeinput2.connected_components.begin(),nodeinput2.connected_components.end(),common_components_between_node.begin());
		common_components_between_node.resize(it-common_components_between_node.begin());
		return sum_conductance(common_components_between_node);

  }*/
}

matrixXd A_inverse inverse_matrix(matrixXd A)
{
	return A.inverse;
}

// inspiration taken from swarthmore.edu
void create_conductance_matrix(network_simulation A, matrixXd G, matrixXd B, matrixXd C, matrixXd D)
{
	MatrixXd A(2,2);
	A(0,0) = G;
	A(0,1) = B;
	A(1,0) = C;
	A(0,1) = D;
}
/* For the below function to work, we need:
- Order the vector network_nodes in order of nodes(0, 1, 2 etc.)
*/
void create_G_matrix(network_simulation A)
{

	//diagonal part of matrix G
	MatrixXd G(A.network_nodes.size(), A.network_nodes.size());
	for(int i=0; i<G.columns(); i++)
	{
		for(int j=0;j<G.rows(); j++)
		{
			if(i == j)
			{
				G(i,j) = A.network_nodes[i].sum_of_conductances;
			}
			else()
			{
				for(int k=0; k<A.network_nodes.size(); k++)
				{
					G(i,j) = -(A.network_nodes[k].sum_of_conductances)
				}
			}
		}
	}

}

void create_B_matrix(network_simulation A)
{
	int count;
	for(int i=; i<A.network_components.size(); i++)
	{
		if(A.network_components[i] == **voltage source**)
		{
			count++;
		}
	}
	MatrixXd B(A.network_nodes.size(),count);
	for(int i=0; i<B.columns(); i++)
	{
		for(int j=0; i<B.rows(); j++)
		{
			for(int k=0; k<v_sources.size(); k++)
			{
				if(i == v_sources[k].positive_node)
				{
					B(i,j) = 1;
				}
				else if(i == v.sources[k].negative_node)
				{
					B(i,j) == -1;
				}
				else()
				{
					B(i,j) == 0;
				}
			}
		}
	}
}

void create_C_matrix(MatrixXd B)
{
	MatrixXd C = B.transpose();
}

void create_D_matrix(network_simulation A)
{
	int count_v_sources;
	for(int i=; i<A.network_components.size(); i++)
	{
		if(A.network_components[i] == **voltage source**)
		{
			count++;
		}
	MatrixXd = (count,count);
}


bool is_a_node_voltage_known(node input, node reference_node){
// check if a node is connected to any voltage sources, if so, chekc whether the other node of the voltage source is the reference node, or connected to another voltage source.
	for(int i = 0 ; i < input.connected_components.size(); i++){
		if(input.connected_components[i].component_name == 'V'){
			if(input.connected_components[i].connected_terminals[0] == reference_node){
				return true;
			}
			if(input.connected_components[i].connected_terminals[0] == input){
				if(input.connected_components[i].connected_terminals[1] == reference_node){
					return true;
				}else{
					bool a;
					a = is_a_node_voltage_known(input.connected_components[i].connected_terminals[1], reference_node);
		            }
			}
		}
	}
	return false;
}

}

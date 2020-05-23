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

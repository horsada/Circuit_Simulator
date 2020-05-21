#include "dependencies.hpp"
#include "simulator.hpp"
//use strtol() to convert the textual representation into a number. This requires us to have the terms resulted from Nodal analysis in the form of  number_nodename.
//Note: strtol() can not be used for semiconductor devices or dependent sources because they would result in coefficients of functions instead of known values.

//This function takes a term resulted from nodal analysis to extract the coefficient of one term from NA.
complex<float> write_conductance_terms_knowing_nodes(node nodeinput1, node nodeinput2, float angular_fre)
{
	complex<float> total_impedance;
	total_impedance = 0;
	//when G11,G22,G33 and etc
	if(nodeinput1.node_index == nodeinput2.node_index){
		if(int i = 0; i < nodeinput1.connected_components.size(); i++)
		{
			if(tellname(nodeinput1.connected_components[i]) != "voltage_dependent_source" &&
			   tellname(nodeinput1.connected_components[i]) != "current_dependent_source" &&
			   tellname(nodeinput1.connected_components[i]) != "independent_source"){
					total_impedance += get_impedance(nodeinput1.connected_components[i], angular_fre);
			//function impedance is not written yet. Also, diode's impedance when not conducting should be 0 in this case.
			}
		}
		complex<float> total_conductance = pow(total_impedance,-1);
		return total_conductance;
	}
	//when G12.G21.G13,G31 and etc.
	if(nodeinput1.node_index != nodeinput2.node_index){
		
		bool first_is_larger_or_equal_in_size;
		if(nodeinput1.connected_components.size()>=nodeinput2.connected_components.size()){
			first_is_larger_or_equal_in_size = 1;
		}else{
			first_is_larger_or_equal_in_size = 0;
		}

		if(first_is_larger_or_equal_in_size == 1){
			vector<complex<float>> common_components_between_node(nodeinput1.connected_components.size()); 
			vector<complex<float>>::iterator it;	
			sort(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end());
			sort(nodeinput2.connected_components.begin(),nodeinput2.connected_components.end());
			it = set_intersection(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end(),nodeinput2.connected_components.begin(),nodeinput2.connected_components.end(),common_components_between_node.begin());
			common_components_between_node.resize(it-common_components_between_node.begin());
			if(int i =0; i< common_components_between_node.size(); i++){
				if(tellname(nodeinput1.connected_components[i]) != "voltage_dependent_source" &&
				   tellname(nodeinput1.connected_components[i]) != "current_dependent_source" &&
				   tellname(nodeinput1.connected_components[i]) != "independent_source"){
						total_impedance += get_impedance(common_components_between_node[i], angular_fre);
				}
			}
		}


		if(first_is_larger_or_equal_in_size == 0){
			vector<complex<float>> common_components_between_node(nodeinput2.connected_components.size());
			vector<complex<float>>::iterator it;
			sort(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end());
			sort(nodeinput2.connected_components.begin(),nodeinput2.connected_components.end());
			it = set_intersection(nodeinput1.connected_components.begin(),nodeinput1.connected_components.end(),nodeinput2.connected_components.begin(),nodeinput2.connected_components.end(),common_components_between_node.begin());
			common_components_between_node.resize(it-common_components_between_node.begin());
			if(int i =0; i< common_components_between_node.size(); i++){
				if(tellname(nodeinput1.connected_components[i]) != "voltage_dependent_source" &&
			       tellname(nodeinput1.connected_components[i]) != "current_dependent_source" &&
	             	tellname(nodeinput1.connected_components[i]) != "independent_source"){
					total_impedance += get_impedance(common_components_between_node[i], angular_fre);
				}
			}
		}
	complex<float> total_conductance = pow(total_impedance,-1);
	return total_conductance;
	}

}

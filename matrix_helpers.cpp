#include "dependencies.hpp"
#include "simulator.hpp"

bool is_a_node_voltage_known(node input, node reference_node) {
  // check if a node is connected to any voltage sources, if so, chekc whether the other node of the voltage source is the reference node, or connected to another voltage source.

  if(input == reference_node){
    return true;
  }

  for(int i = 0 ; i < input.connected_components.size(); i++) {
    if(input.connected_components[i].component_name.at(0) == 'V') {
      if(input.connected_components[i].connected_terminals[0] == reference_node){
        return true;
      }
      if(input.connected_components[i].connected_terminals[0] == input) {
        if(input.connected_components[i].connected_terminals[1] == reference_node){
          return true;
        }
        else{
          //the following part might be a bit wrong, because the function might go back to the input node during recursion
          if(input.connected_components[i].connected_terminals[0]==input) {
            return is_a_node_voltage_known(input.connected_components[i].connected_terminals[1], reference_node);
          }
          if(input.connected_components[i].connected_terminals[1]==input) {
            return is_a_node_voltage_known(input.connected_components[i].connected_terminals[0], reference_node);
          }
        }
      }
    }
  }
  return false;
}


bool r_two_nodes_supernodes(component cmp, node reference_node) {
  // this bool function checks if two nodes should be combined into supernodes, thus resulting in a different value in the current column
  // supernodes should be represented by two rows in the matrix.
  // the first row shows the relationship between the two nodes.
  // the second row shows the sum of the conductance terms of two nodes.
  node node1(1);
  node1 = cmp.connected_terminals[0];
  node node2(2);
  node2 = cmp.connected_terminals[1];

  if(is_a_node_voltage_known(node1, reference_node) == 0 && is_a_node_voltage_known(node2, reference_node)== 0 ){
    return true;
  }
  return false;
}



//!!!!!  Here the first term in connected_terminals vector of the components is assumed to be positive, the second one is negative    !!!!!!
/*
//capacitors need to be replaced by voltage sources at different time steps.
//capacitors are closed circuit in the beginning.
independent_v_source convert_C_to_vsource(C capacitor, double timestep){
  independent_v_source x;
  return x;
}

//inductors are open circuits in the beginning.
//inductors need to be replaced by current sources at different time steps.
independent_i_source convert_L_to_isource(L inductor, double timestep){

	//a timer is needed to produce the current_time in the simulation
	//to use this function, u have to make sure the inductor's connected_terminals's voltages are updated after each time step
	//when simulation starts, L should not be converted to an i source
	double voltage_across_L = inductor.connected_terminals[0].node_voltage - inductor.connected_terminals[1].node_voltage;
	double isource_value;
	isource_value = (voltage_across_L / inductor.component_value)*timestep;
	independent_i_source Ltoisource("Ctois", isource_value, inductor.connected_terminals[0] , inductor.connected_terminals[1]) ;
	return Ltoisource;

}
*/
//the following function is used to tell the currents through a V source or an I source
//it calculates the current resulted from other components (not from the source itself) from connected_terminals[0] to connected_terminals[1]
//output increases when current goes out of terminal[0] , output decreases when current goes out of terminal[1]
double tell_currents(component input){
	double output = 0.0;
	//sum currents going out of terminal[0]
	for(int i = 0 ; i < input.connected_terminals[0].connected_components.size() ; i++){
		if(input.connected_terminals[0].connected_components[i].component_name[0] == 'R'){
			if(input.connected_terminals[0].connected_components[i].connected_terminals[0] == input.connected_terminals[0]){
				output += (input.connected_terminals[0].connected_components[i].connected_terminals[0].node_voltage - input.connected_terminals[0].connected_components[i].connected_terminals[1].node_voltage) / input.connected_terminals[0].connected_components[i].component_value[0];
			}
			if(input.connected_terminals[0].connected_components[i].connected_terminals[1] == input.connected_terminals[0]){
				output += (input.connected_terminals[0].connected_components[i].connected_terminals[1].node_voltage - input.connected_terminals[0].connected_components[i].connected_terminals[0].node_voltage) / input.connected_terminals[0].connected_components[i].component_value[0];
			}
		}
	}
	//sum currents going out of terminal[1]
	for(int c = 0; c< input.connected_terminals[1].connected_components.size(); c++){
		if(input.connected_terminals[1].connected_components[c].component_name[0] == 'R'){
			if(input.connected_terminals[1].connected_components[c].connected_terminals[0] == input.connected_terminals[1]){
				output -= (input.connected_terminals[1].connected_components[c].connected_terminals[0].node_voltage - input.connected_terminals[1].connected_components[c].connected_terminals[1].node_voltage) / input.connected_terminals[1].connected_components[c].component_value[0];
			}
			if(input.connected_terminals[1].connected_components[c].connected_terminals[1] == input.connected_terminals[1]){
				output -= (input.connected_terminals[1].connected_components[c].connected_terminals[1].node_voltage - input.connected_terminals[1].connected_components[c].connected_terminals[0].node_voltage) / input.connected_terminals[1].connected_components[c].component_value[0];
			}
		}
	}
	return output;
}

//the following function should take the version of network_component, where all C and Ls are converted to sources.
//the output of the function includes the current through all components from the input. The orders are matched.
vector<double> calculate_current_through_component(vector<component> network_component, double current_time){
	
	vector<double> current_column;

	//go through all components
	//treat different ones differently
	for(int i = 0 ; i < network_component.size() ; i++){
		
		// the current through a resistor is done by ( the node voltage at connected_terminals[0] - the node voltage at connected_terminals[1]) / resistor value.
		// Also, the i sources connected to the connected_terminals of the resistor are taken into account.
		// To keep it consistent, its always positive.
		if(network_component[i].component_name[0] == 'R'){
			double current_through_R;
			current_through_R = (network_component[i].connected_terminals[0].node_voltage - network_component[i].connected_terminals[1].node_voltage) / network_component[i].component_value[0];
			current_through_R = abs(current_through_R);
			current_column.push_back(current_through_R);
		}
	
		// The current through V shows the current going through V from the positive side of the v source to the negative side of the v source 
		if(network_component[i].component_name[0] == 'V'){
			double current_through_V;
			current_through_V = tell_currents(network_component[i]);
			current_column.push_back(current_through_V);
		}
		
		//The current through I shows the current going through I from the In side to the Out side.
		if(network_component[i].component_name[0] == 'I'){
			double current_through_I;
			current_through_I = tell_currents(network_component[i]);
			double value_of_I;
			value_of_I = network_component[i].component_value[0] + network_component[i].component_value[1]*sin(network_component[i].component_value[2]*current_time);
			current_through_I += value_of_I;
			current_column.push_back(current_through_I);
		}
	}
	
	return current_column;
	
}



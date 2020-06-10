 #include "dependencies.hpp"
#include "simulator.hpp"

bool is_a_node_voltage_known(node input, node reference_node) {
  // check if a node is connected to any voltage sources, if so, check whether the other node of the voltage source is the reference node, or connected to another voltage source.

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

  if(is_a_node_voltage_known(node1, reference_node) == 0 && is_a_node_voltage_known(node2, reference_node)== 0){
    return true;
  }
  return false;
}

double sum_known_currents(node input, double simulation_progress) {
//this function sums up the currents going out of one node at a specific time
	double sum_current = 0.0;
	for(int i = 0 ; i < input.connected_components.size() ; i++){
		if(input.connected_components[i].component_name[0] == 'I'){
			if(input.connected_components[i].connected_terminals[1] == input) {
				sum_current += input.connected_components[i].component_value[0]; // add dc offset
				sum_current += input.connected_components[i].component_value[1]* sin(input.connected_components[i].component_value[2] * simulation_progress); // add amplitude*sin(frequency*time
			}
			if(input.connected_components[i].connected_terminals[0] == input) {
				sum_current -= input.connected_components[i].component_value[0]; // subtract dc offset
				sum_current -= input.connected_components[i].component_value[1]* sin(input.connected_components[i].component_value[2] * simulation_progress); // subtract amplitude*sin(frequency*time
			}
		}
	}
	return sum_current;
}

// Returns pairs of normal nodes, which represent a supernode.
vector<pair<node,node>> supernode_separation(vector<component> components, node reference_node) {
  // A supernode consists of two nodes. In the matrix a supernode occupies two lines. Each of the two nodes is separated
  //this function takes in the components in the network simulation and checks supernodes.
  //This function outputs a vector of vector of nodes.
  //The first vector of nodes contains the relationship node in the supernode, e.g. 1*V2 - 1*V3 = 10
  //The second vector of nodes contains the non relationship node in the supernode, e.g. G11+G21, G12+G22, G13+G23 row
  //This function finds the v sources and does supernode separation

	vector<node> relationship_node;
	vector<node> non_relationship_node;
  vector<pair<node,node>> output;
  for(component cmp: components) {
		if(cmp.component_name[0] == 'V'){
			if(r_two_nodes_supernodes(cmp, reference_node)){
				//the positive side of the V source is going to be the relationship node (assumed to be, it doesnt matter if it's the relationship one or the non relationship one
        output.push_back({cmp.connected_terminals[0], cmp.connected_terminals[1]});
			}
		}
	}
	return output;
}

int which_is_the_node(vector<node> nodes_wo_ref , node input){

	int counter = 0;
	for(int i = 0; i < nodes_wo_ref.size(); i++){
		if(nodes_wo_ref[i] == input){
			counter = i;
		}
	}
	return counter;
}

//!!!!!  Here the first term in connected_terminals vector of the components is assumed to be positive, the second one is negative    !!!!!!
/*
//capacitors need to be replaced by voltage sources at different time steps.
//capacitors are closed circuit in the beginning.
independent_v_source convert_C_to_vsource(C capacitor, double simulation_progress){

}

//inductors are open circuits in the beginning.
//inductors need to be replaced by current sources at different time steps.
independent_i_source convert_L_to_isource(L inductor, double simulation_progress){

	//a timer is needed to produce the current_time in the simulation
	//to use this function, u have to make sure the inductor's connected_terminals's voltages are updated after each time step
	//when simulation starts, L should not be converted to an i source

  // At the start of the simulation, no energy is stored in the inductor. It can be treated as open-circuit or a 0A current source.
  if(simulation_progress == 0.0){
    Ltoisource = independent_i_source("I_"+ inductor.component_name, 0.0, 0.0 ,0.0,inductor.connected_terminals);
    return Ltoisource;
  }

	double voltage_across_L = inductor.connected_terminals[0].node_voltage - inductor.connected_terminals[1].node_voltage;
	double isource_value;
	isource_value = (voltage_across_L / inductor.component_value[0])*timestep;
	independent_i_source Ltoisource("I_"+ inductor.component_name, isource_value, 0.0, 0.0, inductor.connected_terminals);
	return Ltoisource;
}


vector<component> convert_CLs_to_sources(vector<component> network_components, double simulation_progress){
  vector<component> output;
  output = network_components;
  for(int i = 0 ; i < network_components.size(); i++){
    if(network_components[i].component_name[0] == "L"){
      node node1(1);
      node node2(2);
      independent_i_source LtoI("np", 0.0, 0.0 , 0.0, {node1,node2});
      LtoI = convert_L_to_isource(network_components[i], double simulation_progress);
      output.push_back(LtoI);
    }
  }
  // For capacitors, not finished yet
  return output;
}
*/
//function that updates source value !!!



//the following function is used to tell the currents through a V source
//it calculates the current resulted from other components (not from the source itself) from connected_terminals[0] to connected_terminals[1]
//output increases when current goes out of terminal[0] , output decreases when current goes out of terminal[1]
double tell_currents(component input, vector<node> Vvector, double simulation_progress){
	double output = 0.0;
	//sum currents going out of terminal[0]
	int which_is_node0 = which_is_the_node(Vvector, input.connected_terminals[0]);

	for(int i = 0 ; i < Vvector[which_is_node0].connected_components.size() ; i++){
		
		if(Vvector[which_is_node0].connected_components[i].component_name[0] == 'R'){
			if(Vvector[which_is_node0].connected_components[i].connected_terminals[0] == Vvector[which_is_node0]){
				output += calculate_current_through_R(Vvector[which_is_node0].connected_components[i], Vvector);
			}
			if(Vvector[which_is_node0].connected_components[i].connected_terminals[1] == Vvector[which_is_node0]){
				output -= calculate_current_through_R(input.connected_terminals[0].connected_components[i], Vvector);
			}
		}
		if(Vvector[which_is_node0].connected_components[i].component_name[0] == 'I'){
			if(Vvector[which_is_node0].connected_components[i].connected_terminals[0] == Vvector[which_is_node0]){
				output += Vvector[which_is_node0].connected_components[i].component_value[0] + Vvector[which_is_node0].connected_components[i].component_value[1]*sin(Vvector[which_is_node0].connected_components[i].component_value[2]*simulation_progress);
			}
			if(Vvector[which_is_node0].connected_components[i].connected_terminals[1] == Vvector[which_is_node0]){
				output -= Vvector[which_is_node0].connected_components[i].component_value[0] + Vvector[which_is_node0].connected_components[i].component_value[1]*sin(Vvector[which_is_node0].connected_components[i].component_value[2]*simulation_progress);
	
			}
		}
	}
	/*
	//sum currents going out of terminal[1]
	for(int c = 0; c< input.connected_terminals[1].connected_components.size(); c++){
		if(input.connected_terminals[1].connected_components[c].component_name[0] == 'R'){
			if(input.connected_terminals[1].connected_components[c].connected_terminals[0] == input.connected_terminals[1]){
				output -= calculate_current_through_R(input.connected_terminals[0].connected_components[c], Vvector);
			}
			if(input.connected_terminals[1].connected_components[c].connected_terminals[1] == input.connected_terminals[1]){
				output += calculate_current_through_R(input.connected_terminals[0].connected_components[c], Vvector);
			}
		}
		if(input.connected_terminals[0].connected_components[c].component_name[0] == 'I'){
             if(input.connected_terminals[0].connected_components[c].connected_terminals[0] == input.connected_terminals[0]){
                 output -= input.connected_terminals[0].connected_components[c].component_value[0] + input.connected_terminals[0].connected_components[c].component_value[1]*sin(input.connected_terminals[0].connected_components[c].component_value[2]*simulation_progress);
             }
             if(input.connected_terminals[0].connected_components[c].connected_terminals[1] == input.connected_terminals[0]){
                 output += input.connected_terminals[0].connected_components[c].component_value[0] + input.connected_terminals[0].connected_components[c].component_value[1]*sin(input.connected_terminals[0].connected_components[c].component_value[2]*simulation_progress);

             }
         }
	
	}
	*/
	return output;
}

double calculate_current_through_R(component R, vector<node> Vvector){
	//the function calculates the current through R by using the node voltage difference across it divided by R value
	// this function is written because nodes are recreated in Vvector when copying from the network_nodes of the network simulation class
	double node0voltage = 0.0;
	double node1voltage = 0.0;
	double current_through_R;
	for(node no: Vvector){
		if(R.connected_terminals[0].index == no.index){
			node0voltage = no.node_voltage;
		}
		if(R.connected_terminals[1].index == no.index){
			node1voltage = no.node_voltage;
		}
	}
	current_through_R = (node0voltage - node1voltage) / R.component_value[0];
	return current_through_R;	

}
//the following function should take the version of network_component, where all C and Ls are converted to sources.
//the output of the function includes the current through all components from the input. The orders are matched.
vector<double> calculate_current_through_component(vector<component> network_component, vector<node> Vvector, double simulation_progress){

	vector<double> current_column;

	//go through all components
	//treat different ones differently
	for(int i = 0 ; i < network_component.size() ; i++){

		// the current through a resistor is done by ( the node voltage at connected_terminals[0] - the node voltage at connected_terminals[1]) / resistor value.
		// Also, the i sources connected to the connected_terminals of the resistor are taken into account.
		// To keep it consistent, its always positive.
		if(network_component[i].component_name[0] == 'R'){
			double current_through_R = 0.0;
			current_through_R = calculate_current_through_R(network_component[i], Vvector);
			current_through_R = abs(current_through_R);
			current_column.push_back(current_through_R);
		}

		// The current through V shows the current going through V from the positive side of the v source to the negative side of the v source
		if(network_component[i].component_name[0] == 'V'){
			double current_through_V = 0.0;
			current_through_V = tell_currents(network_component[i],Vvector, simulation_progress);
			current_through_V = abs(current_through_V);
			current_column.push_back(current_through_V);
		}

		//The current through I shows the current going through I from the In side to the Out side.
		if(network_component[i].component_name[0] == 'I'){
			double current_through_I = 0.0;
			current_through_I = network_component[i].component_value[0] + network_component[i].component_value[1]*sin(network_component[i].component_value[2]*simulation_progress);
			current_through_I = abs(current_through_I);
			current_column.push_back(current_through_I);
		}

		//!!!!! implement this for C and Ls, probably just output 0? To be decided
	}

	return current_column;

}

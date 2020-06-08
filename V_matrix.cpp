#include "simulator.hpp"
#include "dependencies.hpp"

vector<node> create_v_matrix(network_simulation A)
{
  vector<node> network_nodes_without_ref_node;
  for(int c = 0 ; c < A.network_nodes.size(); c++){
	if(A.network_nodes[c].index != 0 ){

	  network_nodes_without_ref_node.push_back(A.network_nodes[c]);	  
	}
  }
//  int rows;
//  rows = network_nodes_without_ref_node.size();
//	MatrixXd v(rows, 1);
	
//  for(int i=0; i<network_nodes_without_ref_node.size(); i++)
//  {
//      v(i,1) = network_nodes_without_ref_node[i];
//  }
 // return v;
}

double sum_known_currents(node input, double current_time){
//this function sums up the currents going out of one node at a specific time
	double sum_current = 0.0;
	for(int i = 0 ; i < input.connected_components.size() ; i++){
		if(input.connected_components[i].component_name[0] == 'I'){
			if(input.connected_components[i].connected_terminals[0] == input){

				sum_current += input.connected_components[i].component_value[0]; // add dc offset
				sum_current += input.connected_components[i].component_value[1]* sin(input.connected_components[i].component_value[2] * current_time); // add amplitude*sin(frequency*time
			}
			if(input.connected_components[i].connected_terminals[1] == input){

				sum_current -= input.connected_components[i].component_value[0]; // subtract dc offset
				sum_current -= input.connected_components[i].component_value[1]* sin(input.connected_components[i].component_value[2] * current_time); // subtract amplitude*sin(frequency*time



			}
		}
	return sum_current;	
	}

	return 0.0;
}


MatrixXd create_i_matrix(network_simulation A, double current_time)
{
  vector<node> nodes_with_ref_node = A.network_nodes;
  vector<node> nodes_wo_ref_node = create_v_matrix(A);

  int rows = nodes_wo_ref_node;
  MatrixXd i(rows,1);


  for(int it=0; i<A.network_components.size(); it++)
  {
    if(A.network_components[it].component_name.find('i') > -1)
    {

      if(is_a_node_voltage_known(A.network_nodes[it], A.network_nodes[0]) == true)
      {
        i(it,1) = A.network_nodes[it].node_voltage;
      }
      else if(r_two_nodes_supernodes(A.network_nodes[it], A.network_nodes[0], ))
      i(it,1) = A.network_components[it];
    }
  }
  return i;
}


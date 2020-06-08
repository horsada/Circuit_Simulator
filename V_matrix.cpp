#include "simulator.hpp"
#include "dependencies.hpp"

double impedance(component cmp) {
  if (cmp.component_name[0] == 'R') {
    return cmp.read_value()[0];
  }
  return 0.0; // avoids compiler warnings
}

double sum_conductance(vector<component> components) {
  double sum;
  for(component cmp: components) {
    if(cmp.component_name[0] == 'R')
    sum += 1.0/impedance(cmp);
  }
  return sum;
}


double calculate_conductance_between_nodes(node A, node B) {

  // For diagonal conductance matrix entries (G11 ,G22, G33 ...)
  	if(A==B){
  		return sum_conductance(A.connected_components);
  	}

    // For all other conductance matrix entries (G12, G21, G13, G31 ...)
    vector<component> common_components_between_AB;
    for(int i = 0 ; i < A.connected_components.size(); i++){
      for(int c = 0; c < B.connected_components.size(); c++){
        if(A.connected_components[i] == B.connected_components[c]){
          common_components_between_AB.push_back(A.connected_components[i]);
        }
      }
    }
    return sum_conductance(common_components_between_AB);
}

//
// // Not working
// MatrixXd create_G_matrix(network_simulation A) {
//
// 	//diagonal part of matrix G
// 	MatrixXd G(A.network_nodes.size(), A.network_nodes.size());
// 	for(int i=0; i<G.rows(); i++)
// 	{
// 		for(int j=0;j<G.columns(); j++)
// 		{
//       if(is_a_node_voltage_known(A.network_nodes[i], A.network_nodes[0]) == true)
//       {
//         if(i == j)
//         {
//           G(i,j) = 1;
//         }
//         else()
//         {
//           G(i,j) = 0;
//         }
//       }
//       else if(r_two_nodes_supernodes(A.network_nodes[i], A.network_nodes[j], A.network_nodes[0]) == true)
//       {
//         if(A.network_nodes[i].index == i && A.network_nodes[j].index == j)
//         {
//           G(i,j) = voltage_at_supernode(A.network_nodes[i], A.network_nodes[j])
//         }
//         else if(A.network_nodes[i].index == i && A.network_nodes[j].index != j)
//         {
//
//         }
//       }
// 			else if(i == j)
// 			{
// 				G(i,j) = sum_of_conductances(A.network_nodes[i], A);
// 			}
// 			else()
// 			{
// 				G(i,j) = conductance_between_nodes(A.network_nodes[i], A.network_nodes[j], A);
// 			}
// 		}
// 	}
// 	return G;
//
// }


vector<node> create_v_matrix(network_simulation A) {
  vector<node> network_nodes_without_ref_node;
  for(int c = 0 ; c < A.network_nodes.size(); c++){
	if(A.network_nodes[c].index != 0 ) {
      network_nodes_without_ref_node.push_back(A.network_nodes[c]);
    }
  }
  assert(1); return network_nodes_without_ref_node; // to avoid compiler warning
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

	}

	return sum_current;
}

// Returns pairs of normal nodes, which represent a supernode.
vector< pair<node,node> > supernode_separation(vector<component> components, node reference_node) {
  // A supernode consists of two nodes. In the matrix a supernode occupies two lines. Each of the two nodes is separated
  //this function takes in the components in the network simulation and checks supernodes.
  //This function outputs a vector of vector of nodes.
  //The first vector of nodes contains the relationship node in the supernode, e.g. 1*V2 - 1*V3 = 10
  //The second vector of nodes contains the non relationship node in the supernode, e.g. G11+G21, G12+G22, G13+G23 row
  //This function finds the v sources and does supernode separation
	vector<node> relationship_node;
	vector<node> non_relationship_node;
  vector<pair<node,node>> output;
	for(int i = 0 ; i < components.size(); i++){
		if(components[i].component_name[0] == 'V'){
			if(r_two_nodes_supernodes(components[i], reference_node)){
				//the positive side of the V source is going to be the relationship node (assumed to be, it doesnt matter if it's the relationship one or the non relationship one
        output.push_back({components[i].connected_terminals[0], components[i].connected_terminals[1]});
			}
		}
	}
	return output;

}

//
// MatrixXd create_i_matrix(network_simulation A, double current_time)
// {
//   vector<node> nodes_with_ref_node = A.network_nodes;
//   vector<node> nodes_wo_ref_node = create_v_matrix(A);
//
//   int rows = nodes_wo_ref_node;
//   MatrixXd i(rows,1);
//
//   for(int i = 0; i<A.network_components.size();i++){
//
//   }
//
//   for(int it=0; i<A.network_components.size(); it++)
//   {
//     if(A.network_components[it].component_name.find('i') > -1)
//     {
//
//       if(is_a_node_voltage_known(A.network_nodes[it], A.network_nodes[0]) == true)
//       {
//         i(it,1) = A.network_nodes[it].node_voltage;
//       }
//       else if(r_two_nodes_supernodes(A.network_nodes[it], A.network_nodes[0], ))
//       i(it,1) = A.network_components[it];
//     }
//   }
//   return i;
// }

MatrixXd create_i_matrix(network_simulation A, double current_time)
{
  vector<node> nodes_with_ref_node = A.network_nodes;
  vector<node> nodes_wo_ref_node = create_v_matrix(A);
  node reference_node(0);
	
  // the following for loop finds the reference node in the circuit and assign it to the reference_node
  for(int it = 0; it < nodes_with_ref_node.size(); it++){
	if(nodes_with_ref_node[it].index == 0){
		reference_node = nodes_with_ref_node[it];
	}
  }

  //the matrix with 1 column and some rows declared. The number of rows is defined by the number of nodes in the circuit excluding the reference node 
  int rows = nodes_wo_ref_node.size();
  MatrixXd I(rows,1);
  
  //this does supernodes separation, it finds supernodes, separate them into a pair of nodes, relationship node and non-relationship node
  vector<pair<node,node>> supernodes;
  supernodes =  supernode_separation(A.network_nodes, node reference_node);

  //the for loop checks all the nodes and pushes value into the matrix according to different situations.
  for(int i = 0; i<nodes_wo_ref_node.size();i++){
		
	//if it is a known node	
	if(is_a_node_voltage_known(nodes_wo_ref_node[i],reference_node){
		for(int c = 0; c < nodes_wo_ref_node[i].connected_components.size();c++){
			//find the v source that the node is connected to
			if(nodes_wo_ref_node[i].connected_components[c].component_name[0] == 'V'){
				double voltage;
				//voltage is calculated by amplitude*sin(frequency*current time)
				//see if the positive side or the negative side of the v source is conneceted to the node
				//positive side to the node
				if(nodes_wo_ref_node[i].connected_components[c].connected_terminals[0] == nodes_wo_ref_node[i]){
					voltage = nodes_wo_ref_node[i].connected_components[c].component_value[0] + nodes_wo_ref_node[i].connected_components[c].component_value[1]*sin(nodes_wo_ref_node[i].connected_components[c].component_value[2]*current_time);
					I(i,1) = voltage;
				}
				//negative side to the node
			    if(nodes_wo_ref_node[i].connected_components[c].connected_terminals[1] == nodes_wo_ref_node[i]){
					voltage = 0.0 - (nodes_wo_ref_node[i].connected_components[c].component_value[0] + nodes_wo_ref_node[i].connected_components[c].component_value[1]*sin(nodes_wo_ref_node[i].connected_components[c].component_value[2]*current_time);
                    I(i,1) = voltage;
				}	
			}
		}
	 }

	//if it is a relationship supernode
	  for(int s = 0; s < supernode.size(); s++){
		if(nodes_wo_ref_node[i] == supernodes[s].first){
		//the positive side of the v source minus the negative side of the v source is equal to the following voltage
			double voltage;
			voltage = nodes_wo_ref_node[i].connected_components[c].component_value[0] + nodes_wo_ref_node[i].connected_components[c].component_value[1]*sin(nodes_wo_ref_node[i].connected_components[c].component_value[2]*current_time);
			I(i,1) = voltage;
		}
	  }

	//if it is a non relationship supernode
	  for(int s = 0; s < supernodes.size(); s++){ 
		if(nodes_wo_ref_node[i] == supernodes[s].second){
		// return the sum of I sources going out of the node if the supernode 
			double current = 0.0;
			//add the current going out of the first super node
			current += sum_known_currents(supernodes[s].first, current_time);
			// add the current going out of the second super node 
			current += sum_known_currents(supernodes[s].second, current_time);
			I(i,1) = current;
		}
	  }
	

  //if it is a normal node connected to it.
  //two situations are included:1 with I sources connected to it, 2 without I sources connected to it
	 double current;
	 current = sum_known_currents(nodes_wo_ref_node[i], current_time);
	 I(i,1) = current;

    }

  return I;
}


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
    return -sum_conductance(common_components_between_AB);
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

// This functoin constructs the current single-column matrix  (I in G*V = I)
MatrixXd create_i_matrix(network_simulation A, double simulation_progress) {
  vector<node> nodes_with_ref_node = A.network_nodes;
  vector<node> unknown_nodes = create_v_matrix(A);
  node reference_node(0);

  // the following for loop finds the reference node in the circuit and assigns it to the reference_node
  for(int it = 0; it < nodes_with_ref_node.size(); it++){
    if(nodes_with_ref_node[it].index == 0){
      reference_node = nodes_with_ref_node[it];
    }
  }


  //the matrix with 1 column and some rows declared. The number of rows is defined by the number of unknown voltage nodes in the circuit
  int rows = unknown_nodes.size();
  MatrixXd current_matrix(rows,1);

  //this does supernodes separation, it finds supernodes, separate them into a pair of nodes, relationship node and non-relationship node
  vector<pair<node,node>> supernodes = supernode_separation(A.network_components, reference_node);

  // for(int a=0 ; a < A.network_components.size(); a++){
  //   cout << "index is = " << a << endl;
  //   cout << "isthiszero=" << A.network_components[a].connected_terminals[0].connected_components.size() << endl;
  // }

  // The for loop checks all the nodes and pushes value into the matrix according to different situations.
  for(int i = 0; i < unknown_nodes.size(); i++) {
    cout << "following node_index active: " << unknown_nodes[i].index << endl;
    cout << "dbg2;i=" << i << endl;
    // For a regular node (no supernode), the current sources are summed
    double current;
    current = sum_known_currents(unknown_nodes[i], simulation_progress);
    current_matrix(i,0) = current;

    // If it is a node with known voltage, set the current entry to voltage at that node
    if(is_a_node_voltage_known(unknown_nodes[i],reference_node)) {
      cout << "node voltage is known" << endl;
    	for(component cmp: unknown_nodes[i].connected_components) {
    		//find the v source that the node is connected to
    		if(cmp.component_name[0] == 'V'){
    			double voltage;
    			// Voltage is calculated by amplitude*sin(frequency*current time)
    			// Check if the positive side or the negative side of the v source is conneceted to the node

    			// Positive side to the node
    			if(cmp.connected_terminals[0] == unknown_nodes[i]){
    				voltage = cmp.component_value[0] + cmp.component_value[1]*sin(cmp.component_value[2]*simulation_progress);
    				current_matrix(i,0) = voltage;
    			}
    			// Negative side to the node
    		  if(cmp.connected_terminals[1] == unknown_nodes[i]){
    				voltage = 0.0 - (cmp.component_value[0] + cmp.component_value[1]*sin(cmp.component_value[2]*simulation_progress));
            current_matrix(i,0) = voltage;
    			}
    		}
  	   }
     }

    // For a supernode, there are two matrix-line entries. One line represents a relationship between nodes, the other line represents the total conductance/currents of the two nodes forming a supernode

    // For the node-relationship entry, the source value is used.
    for(pair<node,node> snd: supernodes){

      // If the current node is a supernode
    	if(unknown_nodes[i] == snd.first){
        //cout << "node-relationship entry" << endl;
        vector<double> vsource_values;
        //cout << "dbg4=" << snd.first.connected_components.size() << endl;
        // Iterate through the components of the supernode(node 1 of supernode)

        for(component cmp1: unknown_nodes[i].connected_components){
          //cout << "dbg5" << endl;
          // Iterate through the components of the supernode(node 2 of supernode
          int which = which_is_the_node(unknown_nodes, snd.second);
          for(component cmp2: unknown_nodes[which].connected_components){
            //cout << "dbg6" << endl;
            if(cmp1 == cmp2 && cmp1.component_name[0] == 'V' && cmp2.component_name[0] == 'V'){
              // The voltage source that caused the node to be classified as a supernode
              vsource_values = cmp1.component_value;
              //cout << "dbg7" << endl;
            }
          }
        }

        double voltage = vsource_values[0] + vsource_values[1]*sin(vsource_values[2]*simulation_progress);
    		current_matrix(i,0) = voltage;
    	}

      // If it is a non-relationship supernode
    	if(unknown_nodes[i] == snd.second){
        //cout << "non-relationship summed current entry" << endl;
    	  // return the sum of I sources going out of the node if the supernode
    		double current = 0.0;
    		//add the current going out of the first super node
        int which = which_is_the_node(unknown_nodes, snd.first);
    		current += sum_known_currents(unknown_nodes[which], simulation_progress);
    		// add the current going out of the second super node
    		current += sum_known_currents(unknown_nodes[i], simulation_progress);
    		current_matrix(i,0) = current;
    	}
    }

  }

  // Finally return the assembled current matrix
  return current_matrix;
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

//
// MatrixXd create_G_matrix(network_simulation A){
//
// 	vector<node> nodes_with_ref_node = A.network_nodes;
// 	vector<node> unknown_nodes = create_v_matrix(A);
// 	node reference_node(0);
//
// 	// the following for loop finds the reference node in the circuit and assign it to the reference_node
// 	for(int it = 0; it < nodes_with_ref_node.size(); it++){
// 		if(nodes_with_ref_node[it].index == 0){
// 			reference_node = nodes_with_ref_node[it];
// 		}
// 	}
//
// 	//G matrix declared. The number of rows and columns are defined by the number of nodes in the circuit excluding the reference node
// 	int num = unknown_nodes.size();
// 	MatrixXd G(num,num);
//
// 	//this does supernodes separation, it finds supernodes, separate them into a pair of nodes, relationship node and non-relationship node
// 	vector<pair<node,node>> supernodes = supernode_separation(A.network_components, reference_node);
//
// 	//the following two for loops addresses different terms in the G matrix. It defines all columns in one row first then defines the columns in the second row...
// 	//the "row" for-loop checks all the nodes and pushes value into the columns of the row  according to different situations.
// 	//the whole row is first initiated with normal conductance terms, if the row is a known node or a supernode, the whole row will be overwritten.
// 	for(int row = 0; row < num; row++){
//
// 		 //if it is a normal row with all the conductance terms like G11, G12, G13 and etc.
//          // a normal row is not suppose to trigger any of the modifications of the matrix above
//          for(int column = 0;column <num ; column++){
//              G(row,column) = calculate_conductance_between_nodes(unknown_nodes[row],unknown_nodes[column]);
//          }
//
// 		//if it is a known node
// 		if(is_a_node_voltage_known(unknown_nodes[row],reference_node)){
//
// 			for(int column = 0 ; column < num ; column++){
// 				//if the column corresponds to the known node, make it 1
// 				if(column == row){
// 					G(row,column) = 1.0;
// 				}
// 				//if the column corresponds to the other nodes, make it 0
// 				if(column != row){
// 					G(row,column) = 0.0;
// 				}
// 			}
// 		}
//
// 		//if it is a relationship supernode
// 		//the first node in the pair is always on the positive side of the v source
// 		for(int column = 0 ; column < num ; column++){
// 			int which_is_negative_supernode;
// 			for(int s = 0; s< supernodes.size();s++){
// 				if(unknown_nodes[column] == supernodes[s].first && column == row){
// 					//fill the whole row with 0's first
// 					for(int fill_zero = 0; fill_zero < num ; fill_zero++){
// 						G(row,fill_zero) = 0.0;
// 					}
// 					//write things like V2*1 + V3*(-1)
// 					G(row,column) = 1.0;
// 					which_is_negative_supernode = which_is_the_node(unknown_nodes, supernodes[s].second);
// 					G(row,which_is_negative_supernode) = -1.0;
// 				}
// 			}
// 		}
//
// 		//if it is a non relationship supernode
// 		for(int column = 0; column <num ; column++){
// 			int which_is_positive_supernode;
// 			for(int s = 0 ; s< supernodes.size(); s++){
// 				if(unknown_nodes[column] == supernodes[s].second && column == row){
// 				    //fill the whole row with 0's first
//                     for(int fill_zero = 0; fill_zero < num ; fill_zero++){
//                          G(row,fill_zero) = 0.0;
// 	                }
// 					which_is_positive_supernode = which_is_the_node(unknown_nodes, supernodes[s].first);
// 					//!!!!!!!!!!!!    sums the conductances
// 				    for(int double_column = 0 ; double_column < num ; double_column++){
// 						G(row,double_column) = calculate_conductance_between_nodes(unknown_nodes[row],unknown_nodes[double_column]) + calculate_conductance_between_nodes(unknown_nodes[which_is_positive_supernode],unknown_nodes[double_column]);
// 					}
// 				}
//
// 			}
//
// 		}
//
//
// 	}
// 	return G;
// }


/*

i=0
dbg2
i=1
dbg2
i=2
dbg2
Segmentation fault: 11

*/

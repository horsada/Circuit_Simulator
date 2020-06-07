#include "simulator.hpp"
#include "dependencies.hpp"

MatrixXd create_v_matrix(network_simulation A)
{
  vector<node> network_nodes_without_ref_node;
  for(int c = 0 ; c < A.network_nodes.size(); c++){
	  if(A.network_nodes[c].index != 0){
		  network_nodes_without_ref_node.push_back(A.network_nodes[c]);
	  }
  }
  rows = network_nodes_without_ref_node.size();
	MatrixXd v(rows, 1);

  for(int i=0; i<network_nodes_without_ref_node.size(); i++)
  {
      v(i,1) = network_nodes_without_ref_node[i];
  }
  return v;
}

MatrixXd create_i_matrix(network_simulation A)
{
  int rows = A.no_of_i_sources;
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

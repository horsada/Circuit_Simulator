#include "simulator.hpp"
#include "dependencies.hpp"

double impedance(component com){
  if (com.component_name[0] == 'R') {
    return com.read_value()[0];
  }
}

double sum_conductance(vector<component> A) {
  double sum;
  for(int i=0; i<A.size(); i++)
  {
    if(A[i].component_name[0] == 'R')
    sum += 1.0/ impedance(A[i]);
  }
  return sum;
}

double calculate_conductance_between_nodes(node A, node B) {

  // For diagonal conductance matrix entries (G11 ,G22, G33 ...)
  	if(A.node_index == B.node_index){
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


/*

  for(int i=0; i<max(A.connected_components.size(), B.connected_components.size()); i++)
  {
    for(int j=0; j<max(A.connected_components.size(), B.connected_components.size()); j++)
    {
      if(A.connected_components[i] == B.connected_components[j])
      {
        if(A.connected_components[i].component_name.find(r) > -1)
        {
          return A.connected_components[i].impedance(C);
        }
      }
    }
  }
  */


MatrixXd create_G_matrix(network_simulation A)
{

	//diagonal part of matrix G
	MatrixXd G(A.network_nodes.size(), A.network_nodes.size());
	for(int i=0; i<G.rows(); i++)
	{
		for(int j=0;j<G.columns(); j++)
		{
      if(is_a_node_voltage_known(A.network_nodes[i], A.network_nodes[0]) == true)
      {
        if(i == j)
        {
          G(i,j) = 1;
        }
        else()
        {
          G(i,j) = 0;
        }
      }
      else if(r_two_nodes_supernodes(A.network_nodes[i], A.network_nodes[j], A.network_nodes[0]) == true)
      {
        if(A.network_nodes[i].index == i && A.network_nodes[j].index == j)
        {
          G(i,j) = voltage_at_supernode(A.network_nodes[i], A.network_nodes[j])
        }
        else if(A.network_nodes[i].index == i && A.network_nodes[j].index != j)
        {

        }
      }
			else if(i == j)
			{
				G(i,j) = sum_of_conductances(A.network_nodes[i], A);
			}
			else()
			{
				G(i,j) = conductance_between_nodes(A.network_nodes[i], A.network_nodes[j], A);
			}
		}
	}
	return G;

}

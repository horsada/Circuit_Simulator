#include "simulator.hpp"
#include "dependencies.hpp"

complex<float> r::impedance(network_simulation A)
{
  double resistance = this->component_value;
  complex<float> impedance = (component_value, 0);
  return impedance;
}

complex<float> c::impedance(network_simulation A)
{
  double capacitance = this->component_value;
  complex<float> impedance = -1/A.ang_freq*capacitance;
}

complex<float> l::impedance(network_simulation A)
{
  double inductance = this->component_value;
  complex<float> impedance = -1/A.ang_freq*capacitance;
}

double sum_of_conductances(node A, network_simulation B)
{
  complex<float> sum;
  for(int i=0; i<A.connected_components; i++)
  {
    if(A.connected_components.component_name.find(r) > -1)
    sum += A.connected_components[i].impedance(B);
  }
  return sum;
}

double conductance_between_nodes(node A, node B, network_simulation C)
{

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
}

MatrixXd create_G_matrix(network_simulation A)
{

	//diagonal part of matrix G
	MatrixXd G(A.network_nodes.size(), A.network_nodes.size());
	for(int i=0; i<G.rows(); i++)
	{
		for(int j=0;j<G.columns(); j++)
		{
			if(i == j)
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

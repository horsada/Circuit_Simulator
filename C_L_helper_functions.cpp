#include "simulator.hpp"

using namespace std;

//!!!!!  Here the first term in connected_terminals vector of the components is assumed to be positive, the second one is negative    !!!!!!

double sum_currents(node input_node){



}

//capacitors need to be replaced by voltage sources at different time steps.
//capacitors are closed circuit in the beginning.
independent_v_source convert_C_to_vsource(C capacitor, double timestep){





}





//inductors are open circuits in the beginning.
//inductors need to be replaced by current sources at different time steps.
independent_i_source convert_L_to_isource(L inductor, double timestep){

		




}

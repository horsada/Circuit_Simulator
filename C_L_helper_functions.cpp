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

	//a timer is needed to produce the current_time in the simulation
	//to use this function, u have to make sure the inductor's connected_terminals's voltages are updated after each time step
	double voltage_across_L = inductor.connected_terminals[0].node_voltage - inductor.connected_terminals[1].node_voltage;
	double isource_value;
	isource_value = (voltage_across_L / inductor.component_value)*timestep;
	independent_i_source Ltoisource("Ctois", isource_value, inductor.connected_terminals[0] , inductor.connected_terminals[1]) ;
	return Ltoisource;

}

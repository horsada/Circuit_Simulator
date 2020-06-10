#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

void write_csv_for_output_at_first_row(string filename, vector<node> nodes, vector<component> network_component){
//this function writes a CSV file with node_index and component names at the top of each column
	ofstream file;
	file.open(filename);
	file << "Time" << "," ; 
	for(int i = 0 ; i < nodes.size(); i++){
		file <<  nodes[i].index << "," ; 
	}
	for(int c = 0; c < network_component.size() ; c++){
		file << network_component[c].component_name;
		if(c < network_component.size -1){
			file << "," ;
		}
	}
	file.close();
}

void write_csv_for_node_voltages_at_other_rows(string filename, double current_time, vector<node> nodes){ 
//this functin writes the node voltages values at other rows
	
	ofstream file;
	file.open(filename);
	file << endl; // starts at a new line 
	file << current_time << "," ; // write the time 
	//write the node voltages 
	for(int i = 0 ; i < nodes.size(); i++){
		file << nodes[i].node_voltage << "," ;
	}
	file.close();
}

void write_csv_for_component_currents_at_other_rows(string filename, vector<double> current_cmps){

	ofstream file;
	file.open(filename);
	for(int i = 0 ; i < current_cmps.size(); i++){
		file << current_cmps[i];
		if(i < current_cmps.size()-1){
			file << "," ;
		}
	}
	file.close();

}

int main(){
	//create a csv file called output.csv
	ofstream output("output.csv")
	output.close();
	network_simulation sim;

	//the process of how sim is produced by using netlist paser is not done !!!!
	double time_step = sim.timestep;
	double stoptime = sim.stop_time;
	double current_time = 0;
	vector<double> current_through_cmps;
	vector<component> networkcomponent = sim.network_components; // !!!!! This line needs to be changed after C and L are added
	MatrixXd Imatrix;
	MatrixXd Gmatrix;
	MatrixXd Vmatrix;
	vector<node> Vvector = create_v_matrix(sim);
	// V = G.inverse() * I
	// create the first row of the output file
	write_csv_for_output_at_first_row("output.csv", Vvector, networkcomponent)
	// time is increamenting
	// at each time,
	// 1.solve the matrix.
	// 2.push the node_voltages into nodes
	// 3.write the node_voltage into CSV file
	// 4.calculate the current through each component
	// 5.write the currents into a CSV file
	while(current_time < stoptime){
		Imatrix = create_i_matrix(sim,current_time);
		Gmatrix = create_G_matrix(sim);
		Gmatrix = Gmatrix.inverse(); //Get the inverse of the G matrix
		Vmatrix = Gmatrix * Imatrix; // 1. solve the matrix 
		for(int i = 0 ; i < Vvector.size() ; i++){
			Vvector[i].node_voltage = Vmatrix(i,0); // 2. pushing node_voltages into nodes 
		}
		write_csv_for_node_voltages_at_other_rows("output.csv", current_time, Vvector); // 3. write the node_voltage into CSV file
		 
		current_through_cmps = calculate_current_through_component(networkcomponent, current_time); // 4. calculate the current through each component

		write_csv_for_component_currents_at_other_rows("output.csv",current_through_cmps); // 5. write the currents into a CSV file
	    
		current_time += time_step;
	}
}

#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

void write_csv_column_specifiers(string filename, vector<node> nodes, vector<component> network_component) {

	//this function writes a CSV file with node_index and component names at the top of each column
	ofstream ofs;
	ofs.open(filename, ios_base::app);

	ofs << "Time" << "," ;
	for(node nd: nodes){
		ofs <<  nd.index << "," ;
	}
	for(int c = 0; c < network_component.size() ; c++){
		ofs << network_component[c].component_name;
		if(c < network_component.size()-1){
			ofs << "," ;
		}
	}

	ofs << endl;
}

// This functin writes the node voltages values at other rows
void write_csv_voltage_row(string filename, double simulation_progress, vector<node> nodes){
	ofstream ofs;
	ofs.open(filename, ios_base::app);


	ofs << simulation_progress << "," ; // write the time
	//write the node voltages
	for(int i = 0 ; i < nodes.size(); i++){
		ofs << nodes[i].node_voltage << "," ;
	}

}

void write_csv_current_row(string filename, vector<double> current_cmps){
	ofstream ofs;
	ofs.open(filename, ios_base::app);


	for(int i = 0 ; i < current_cmps.size(); i++){
		ofs << current_cmps[i];
		if(i < current_cmps.size()-1){
			ofs << "," ;
		}
	}

	ofs << endl;

}

int main(){
	//create a csv file called output.csv
	ofstream output("output.csv");
	output.close();

	network_simulation sim;
	parse_netlist_line(sim, ".tran 0 10 0 1s");
	parse_netlist_line(sim, "V1 N001 0 5");
	parse_netlist_line(sim, "I2 N003 N004 7");
	parse_netlist_line(sim, "R1 N001 N002 10");
	parse_netlist_line(sim, "R2 N002 N003 15");
	parse_netlist_line(sim, "R3 0 N004 20");
	parse_netlist_line(sim, ".end");

	//the process of how sim is produced by using netlist paser is not done !!!!
	double time_step = sim.timestep;
	double stoptime = sim.stop_time;
	double simulation_progress = 0;
	vector<double> current_through_cmps;
	vector<component> networkcomponents = sim.network_components; // !!!!! This line needs to be changed after C and L are added
	MatrixXd Imatrix;
	MatrixXd Gmatrix;
	MatrixXd Vmatrix;
	vector<node> Vvector = create_v_matrix(sim);
	// V = G.inverse() * I
	// create the first row of the output file
	write_csv_column_specifiers("output.csv", Vvector, networkcomponents);
	// time is increamenting
	// at each time,
	// 1.solve the matrix.
	// 2.push the node_voltages into nodes
	// 3.write the node_voltage into CSV file
	// 4.calculate the current through each component
	// 5.write the currents into a CSV file
	while(simulation_progress < stoptime){
		cout << "simulation_progress=" << simulation_progress;
		Imatrix = create_i_matrix(sim,simulation_progress);
		Gmatrix = create_G_matrix(sim);
		Gmatrix = Gmatrix.inverse(); //Get the inverse of the G matrix
		Vmatrix = Gmatrix * Imatrix; // 1. solve the matrix
		for(int i = 0 ; i < Vvector.size() ; i++){
			Vvector[i].node_voltage = Vmatrix(i,0); // 2. pushing node_voltages into nodes
		}

		write_csv_voltage_row("output.csv", simulation_progress, Vvector); // 3. write the node_voltage into CSV file

		current_through_cmps = calculate_current_through_component(networkcomponents, simulation_progress); // 4. calculate the current through each component

		write_csv_current_row("output.csv",current_through_cmps); // 5. write the currents into a CSV file

		simulation_progress += time_step;
	}
}

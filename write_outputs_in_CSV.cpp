#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

// The input netlist file path
string input_file_name = "netlist.txt";
// The csv output file path
string output_file_name = "output.csv";


void write_csv_column_specifiers(string filename, vector<node> nodes, vector<component> network_component) {

	//this function writes a CSV file with node_index and component names at the top of each column
	ofstream ofs;
	ofs.open(filename);

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
	cout << endl << endl << "ℹ️⚡️ Running Wuyang, Adam & Timeo's Circuit simulator" << endl << endl;
	cout << "🚀🚀🚀 Starting simulation" << endl;

	cout << "Netlist input: " << input_file_name << endl << "CSV Output: " << output_file_name << endl;
	network_simulation sim;

	fstream newfile;
	newfile.open(input_file_name, ios::in);
	if (newfile.is_open()){
		string tp;
		while(getline(newfile, tp)){
			parse_netlist_line(sim, tp);
		}
		newfile.close();
    }
	cout << "🔄 Netlist parsing complete. Running simulation with following paramters: ";

	//the process of how sim is produced by using netlist paser is not done !!!!
	double time_step = sim.timestep;
	double stoptime = sim.stop_time;

	cout << "time_step=" << time_step << "; stoptime=" << stoptime << endl << endl;


	// Converting conductors and capacitors to their source equivalents
	convert_CLs_to_sources(sim);

	// The voltage vector containing unknown voltage nodes
	vector<node> Vvector = create_v_matrix(sim);

	// Writing the column names into the CSV file; This happens before C/L are converted to sources.
	write_csv_column_specifiers(output_file_name, Vvector, sim.network_components);


  //find the equivalent sources in sim.network_components
	//find their connected nodes.
	//add the equivalent sources in to the vector of components in sim.network_nodes

	/*
		Simulation Loop
			1 Solve the matrix equation
			2 Write the calculated voltages to CSV
			3 Calculate currents through components
			4 Write currents to CSV
			5 Update the source equivalents for inductors and capacitors
	*/

	for(double simulation_progress=0; simulation_progress<=stoptime; simulation_progress+=time_step) {

		// 1 Solve the matrix equation
		MatrixXd Imatrix = create_i_matrix(sim,simulation_progress);
		MatrixXd Gmatrix = create_G_matrix(sim);
		Gmatrix = Gmatrix.inverse();
		MatrixXd Vmatrix = Gmatrix * Imatrix;

		for(int i = 0 ; i < Vvector.size() ; i++){
			// Updating node_voltage values in Vvector
			Vvector[i].node_voltage = Vmatrix(i,0);
		}

		
		// 2 Write the calculated voltages to CSV
		write_csv_voltage_row(output_file_name, simulation_progress, Vvector);

		// 3 Calculate currents through components
		vector<double> current_through_cmps = calculate_current_through_component(sim.network_components, Vvector, simulation_progress);

		// 4 Write currents to CSV
		write_csv_current_row(output_file_name, current_through_cmps);

		// 5 Update the source equivalents for inductors and capacitors
		update_source_equivalents(sim, Vvector, current_through_cmps, simulation_progress, time_step);

	}

	MatrixXd Imatrix = create_i_matrix(sim,stoptime); 
	MatrixXd Gmatrix = create_G_matrix(sim);

	cout<< Gmatrix;

	cout<< endl << Imatrix;
	cout << "✅ Simulation Complete ✅" << endl << "📄 Outputs written to: " << output_file_name << endl << endl;
	return 0;
}

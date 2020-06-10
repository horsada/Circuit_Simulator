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

	cout << "Netlist parsing complete. Running simulation with following paramters: " << endl;

	//the process of how sim is produced by using netlist paser is not done !!!!
	double time_step = sim.timestep;
	double stoptime = sim.stop_time;

	cout << "time_step=" << time_step << "; stoptime=" << stoptime << endl << endl;

	double simulation_progress = 0;
	vector<double> current_through_cmps;

	vector<component> networkcomponents = sim.network_components; // !!!!! This line needs to be changed after C and L are added
  // conver cl to source(networkcomponents)
  
	MatrixXd Imatrix;
	MatrixXd Gmatrix;
	MatrixXd Vmatrix;
	vector<node> Vvector = create_v_matrix(sim);
	// V = G.inverse() * I
	// create the first row of the output file
	write_csv_column_specifiers(output_file_name, Vvector, networkcomponents);
	// time is increamenting
	// at each time,
	// 1.solve the matrix.
	// 2.push the node_voltages into nodes
	// 3.write the node_voltage into CSV file
	// 4.calculate the current through each component
	// 5.write the currents into a CSV file
	while(simulation_progress < stoptime){

    
		Imatrix = create_i_matrix(sim,simulation_progress);
		Gmatrix = create_G_matrix(sim);
		Gmatrix = Gmatrix.inverse(); //Get the inverse of the G matrix
		Vmatrix = Gmatrix * Imatrix; // 1. solve the matrix
		for(int i = 0 ; i < Vvector.size() ; i++){
			Vvector[i].node_voltage = Vmatrix(i,0); // 2. pushing node_voltages into nodes
		}
		
		cout << Imatrix;

		write_csv_voltage_row(output_file_name, simulation_progress, Vvector); // 3. write the node_voltage into CSV file

			
		current_through_cmps = calculate_current_through_component(networkcomponents,Vvector, simulation_progress); // 4. calculate the current through each component

		write_csv_current_row(output_file_name, current_through_cmps); // 5. write the currents into a CSV file
    
    //function that updates the value of the sources that CLs become
    //it itrates through all components find the ones which start with "I_" or "V_".
    //change their value based on pervious condition.

		simulation_progress += time_step;
	}
}




/*


Time,1,2,3,I1,R1,R2,R3
0,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.001,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.002,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.003,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.004,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.005,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.006,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.007,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.008,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.009,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.01,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.011,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.012,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.013,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.014,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.015,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.016,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.017,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.018,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.019,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.02,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.021,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.022,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317
0.023,225,175,100,5,7.43303e-317,4.95535e-317,3.71651e-317


* This is a test SPICE file.
.tran 0 10s 0 1ms
I1 N001 0 5
R1 N001 N002 10
R2 N002 N003 15
R3 N003 0 20
.end


*/

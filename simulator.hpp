#ifndef spice_hpp
#define spice_hpp

#include "dependencies.hpp"

/*////////////////////////////////////
////     Core data-structures     ////
////////////////////////////////////*/

// Forward declarations
class node;
class component;
class independent_v_source;


class network_simulation {
  public:
    double stop_time; // Duration of simulation
    double timestep; // Temporal Resolution of simulation
    vector<component> network_components;
    vector<node> network_nodes;
/*
	void convert_cls_to_sources(){
		for(int i = 0 ; i < network_components.size(); i++){
			 if(network_components[i].component_name[0] == 'L'){
			independent_i_source equivalent_source("I_"+network_components[i].component_name, 0.0, 0.0 , 0.0, network_components[i].connected_terminals);
      //network_components[i] = equivalent_source;
			network_components.push_back(equivalent_source);
			}
			else if(network_components[i].component_name[0] == 'C'){
			independent_v_source equivalent_source("V_"+network_components[i].component_name, 0.0, 0.0 , 0.0, network_components[i].connected_terminals);
      //network_components[i] = equivalent_source;
			network_components.push_back(equivalent_source);
			}
		}
*/
};


class node {
  public:
    int index;
    double sum_of_conductances;
    double node_voltage;
    vector<double> conductances;
    vector<component> connected_components;
    ~node(){};
    node(int node_index) {
      index = node_index;
    }
    // operator overload needed to check if two nodes are the same
    bool operator==(const node& other_node) const {
      return this->index == other_node.index;
    }
    void set_sum_of_conductances(double calculated_sum) {
      sum_of_conductances = calculated_sum;
    }
};

class component {
  public:
    string component_name;
    vector<double> component_value;
    vector<node> connected_terminals;

    ~component(){};
    vector<double> read_value() const {
      return component_value;
    };
  	bool operator==(const component& other_component) const {
  		return this->component_name == other_component.component_name;
  	}
};


/*/////////////////////////////////////////
//// Resistors, Capacitors, Inductors  ////
/////////////////////////////////////////*/
class R: public component {
  public:
    R(string device_name, double value, vector<node> connected_nodes) {
      component_name = device_name;
      component_value = {value};
      connected_terminals = connected_nodes;
    }
};


class C: public component {
  public:
    C(string device_name, double value, vector<node> connected_nodes) {
      component_name = device_name;
      component_value = {value};
      connected_terminals = connected_nodes;
    }
};

class L: public component {
  public:
    L(string device_name, double value, vector<node> connected_nodes) {
      component_name = device_name;
      component_value = {value};
      connected_terminals = connected_nodes;
    }
};


/*///////////////////////////
////       SOURCES       ////
///////////////////////////*/

// All sources are treated as AC sources, for DC the frequency, and amplitude is zero, and DC offset is set.
class independent_v_source: public component {
  public:

    independent_v_source(string device_name, double dc_offset_from_netlist, double amplitude_from_netlist, double frequency_from_netlist, vector<node> connected_nodes){
  	  component_name = device_name;
      connected_terminals = connected_nodes;
      component_value = {dc_offset_from_netlist, amplitude_from_netlist, frequency_from_netlist};
    }

};

class independent_i_source: public component {
  public:

  	independent_i_source(string device_name, double dc_offset_from_netlist, double amplitude_from_netlist, double frequency_from_netlist, vector<node> connected_nodes){
  	  component_name = device_name;
      connected_terminals = connected_nodes;
      component_value = {dc_offset_from_netlist, amplitude_from_netlist, frequency_from_netlist};
    }

};


/*////////////////////////////
//// ADVANCED COMPONENTS  ////
////////////////////////////*/
class diode: public component {
    string model_name;
    diode(string device_name, vector<node> connected_nodes, string model_name_from_netlist) {
      component_name = device_name;
      connected_terminals = connected_nodes;
      model_name = model_name_from_netlist;
    }
};

class transistor: public component {
   string model_name;
};

/*//////////////////////////////
//// FUNCTION DECLARATIONS  ////
//////////////////////////////*/

// This functions solves a matrix equation Ax=B (Gv=i)
// It takes any-sized float matrix A and B as an input and computes x.
// MatrixXf solve_matrix_equation(MatrixXf A, MatrixXf B);

// This function takes in a string suffix value (7.2k, 25m, 15Meg) and converts to a double.
double suffix_parser(string prefix_value);

// This converts a raw node name from the netlist to the pure node index (int)
int parse_node_name_to_index(string node_name);

// Takes a netlist line and processes it
int parse_netlist_line(network_simulation &netlist_network, string netlist_line);

// Adds nodes to a network, if they don't exist already
void push_nodes_with_component(network_simulation &netlist_network, vector<node> new_nodes, component new_cmp);

// Returns the impedance of a resistor
double impedance(component cmp);

//This function takes two nodes and return the conductance term in the conductance matrix.
double calculate_conductance_between_nodes(node nodeinput1, node nodeinput2);

// Sums all the conductances of a vector of components
double sum_conductance(vector<component> components);

// The v column, consists of all the voltage nodes in the circuit, excluding the 0 reference node.
vector<node> create_v_matrix(network_simulation A);

// Returns vector of pairs of two regular nodes, which together form a supernode.
vector<pair<node,node>> supernode_separation(vector<component> components, node reference_node);

// This function sums all the known currents (from current sources) at at node. Positive for net outflow, negative for net inflow.
double sum_known_currents(node input, double simulation_progress);


/// ^^^^^tested until here^^^^^

bool is_a_node_voltage_known(node input, node reference_node);

bool r_two_nodes_supernodes(component cmp, node reference_node);

MatrixXd create_i_matrix(network_simulation A, double current_time);

int which_is_the_node(vector<node> nodes_wo_ref, node input);

MatrixXd create_G_matrix(network_simulation A);

double tell_currents(component input, vector<node> Vvector, double simulation_progress);

vector<double> calculate_current_through_component(vector<component> network_component, vector<node> Vvector, double current_time);

double calculate_current_through_R(component R, vector<node> Vvector);

void convert_CLs_to_sources(network_simulation &sim);

vector<component> update_source_equivalents(vector<component> network_components, vector<node> Vvector, vector<double> current_through_components, double simulation_progress, double timestep);

int which_is_cmp(vector<component> networkcmp, component input);


#endif

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
    double ang_freq;

    int no_of_v_sources();
    int no_of_nodes();
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
    vector<node> connected_terminals;
    virtual ~component(){};
<<<<<<< HEAD
    virtual double const read_value() const=0;
    virtual get_impedance() =0; // This needs to be implemented everywhere in order to be pure-virtual
=======
    virtual double const read_value() const=0; // This needs to be implemented everywhere in order to be pure-virtual
	bool operator==(const component& other_component) const {
		bool output = false;
		if(component.component_name == other_component.component_name){
			output = true;
		}
		return output;
	}
>>>>>>> 1546c5643bf49e1ae924250f3600f3c567ada12f
};

/*/////////////////////////////////////////
//// Resistors, Capacitors, Inductors ////
/////////////////////////////////////////*/
class R: public component {
  private:
      double component_value;
      string component_name;
  public:
    R(string device_name, double value, vector<node> connected_nodes) {
      component_name = device_name;
      component_value = value;
      connected_terminals = connected_nodes;
    }
    double const complex_read_value() const {
      return complex<double> = (component_value, 0);
    };
    complex<float> r::impedance(network_simulation A);
};


class C: public component {
private:
	double component_value;
public:
  C(string device_name, double value, vector<node> connected_nodes) {
    component_name = device_name;
    component_value = value;
    connected_terminals = connected_nodes;
  }
  double const read_value() const{
    return component_value;
  }
  complex<float> c::impedance(network_simulation A);
};

class L: public component {
private:
	double component_value;
public:
  L(string device_name, double value, vector<node> connected_nodes) {
    component_name = device_name;
    component_value = value;
    connected_terminals = connected_nodes;
  }
  double const read_value() const{
    return component_value;
  }
  complex<float> l::impedance(network_simulation A);
};


/*///////////////////////////
////       SOURCES       ////
///////////////////////////*/

// All sources are treated as AC sources, for DC the frequency, and amplitude is zero, and DC offset is set.
class independent_v_source: public component, public network_simulation {
public:
    double dc_offset;
    double amplitude;
    double frequency;
  public:
    independent_v_source(string device_name, double dc_offset_from_netlist, double amplitude_from_netlist, double frequency_from_netlist, vector<node> connected_nodes){
  	  component_name = device_name;
      connected_terminals = connected_nodes;

      dc_offset = dc_offset_from_netlist;
      amplitude = amplitude_from_netlist;
      frequency = frequency_from_netlist;
    }
    double const read_value() const {
      cout << this->dc_offset << endl;
      return dc_offset;
    }
};

class independent_i_source: public component, public network_simulation {
public:
    double dc_offset;
    double amplitude;
    double frequency;
  public:
  	independent_i_source(string device_name, double dc_offset_from_netlist, double amplitude_from_netlist, double frequency_from_netlist, vector<node> connected_nodes){
  	  component_name = device_name;
      connected_terminals = connected_nodes;

      dc_offset = dc_offset_from_netlist;
      amplitude = amplitude_from_netlist;
      frequency = frequency_from_netlist;
    }
    double const read_value() const {
      cout << this->dc_offset << endl;
      return dc_offset;
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
void push_nodes(network_simulation &netlist_network, vector<node> new_nodes);

//This get_impedance function is overloaded to return the impedance of several different component
double get_impedance(R r);
//double get_impedance(C c);
//double get_impedance(L l);
//double get_impedance(diode d);
//double get_impedance(BJT b);
//double get_impedance(MOSFET m);

//This function takes two nodes and return the conductance term in the conductance matrix.
double calculate_conductance_between_nodes(node nodeinput1, node nodeinput2);

// Sums all the conductances of a vector of components
double sum_conductance(vector<component> components);

bool is_a_node_voltage_known(node input, node reference_node);

bool r_two_nodes_supernodes(node node1, node node2, node reference_node);
#endif

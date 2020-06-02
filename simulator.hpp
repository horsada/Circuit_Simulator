#ifndef spice_hpp
#define spice_hpp

#include "dependencies.hpp"

/*///////////////////////
  CORE DATA STRUCTURE
///////////////////////-*/

// Forward declarations
class node;
class component;

class network_simulation {
  double stop_time; // Duration of simulation
  double timestep; // Temporal Resolution of simulation
  vector<component> network_components;
  vector<node> network_nodes;
  vector<independent_v_source> v_sources;
};

class node {
  public:
  	int index; // changed to int for read_file.cpp
    double sum_of_conductances;
    vector<double> conductances;
  	vector<component> connected_components;
    node(){};
    node(int node_index)
    {
      index = node_index;
    }
    void sum_of_conductances()
    {
      // assign member variable sum_of_conductances using this function
    }
};

class component {
  public:
    string component_name;
    vector<node> connected_terminals;
    virtual double read_value() =0;
    int positive_node;
    int negative_node;
};

class R: public component {
  private:
      double component_value;
  public:
    double read_value(){
      return component_value;
    }

    // Adam - for read_file.cpp
    R(string device_name, double value) {
      component_name = device_name;
      component_value = value;
    }
};


class C: public component {
private:
	double component_value;
public:
  // Adam - for read_file.cpp
  C(string device_name, double value) {
    component_name = device_name;
    component_value = value;
  }
};

class L: public component {
private:
	double component_value;
public:
  // Adam - for read_file.cpp
  L(string device_name, double value) {
    component_name = device_name;
    component_value = value;
  }
};



//check how temperature affects the parameters
class diode :public component {
    string diodename;
};

class BJT:public component {
   string BJT_name;
   float beta;
};

class MOSFET :public component {
    string MOSFETname;
    float length;
    float width;
};

class independent_v_source : public components
{
  double value;
  double frequency;
}

/*/////////////////////////
  FUNCTION DECLARATIONS
/////////////////////////-*/

// This functions solves a matrix equation Ax=B (Gv=i)
// It takes any-sized float matrix A and B as an input and computes x.
// MatrixXf solve_matrix_equation(MatrixXf A, MatrixXf B);

// This function takes in a string suffix value (7.2k, 25m, 15Meg) and converts to a double.
double suffix_parser(string prefix_value);

// Takes a netlist line and processes it
int parse_netlist_line(network_simulation netlist_network, string netlist_line);

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

#endif

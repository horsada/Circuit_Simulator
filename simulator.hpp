#ifndef spice_hpp
#define spice_hpp

#include "dependencies.hpp"

/*///////////////////////
  CORE DATA STRUCTURE
///////////////////////*/

// Forward declarations
class node;
class component;

struct network_simulation {
  double stop_time; // Duration of simulation
  double timestep; // Temporal Resolution of simulation
  vector<component> network_components;
  vector<node> network_nodes;
};

class node{
  public:
	string node_index;
	vector<component> connected_components;
  node(){};
};

class component {
  public:
    
    component(){};
    string component_name;
    vector<node> connected_terminals;
    component(string netlist_line);
    virtual double read_value();

};

class R: public component
{
  protected:
    double component_value;
  public:
    R(double value){
      component_value = value;
    };
    double read_value(){
      return component_value;
    }
};


class C: public component
{
	protected:
	double component_value;
};

class L: public component
{
	protected:
	double component_value;
};



//check how temperature affects the parameters
class diode
  :public component
  {
    string diodename;
  };

class BJT
  :public component
  {
   string BJT_name;
   float beta;
  };

class MOSFET
  :public component
  {
    string MOSFETname;
    float length;
    float width;
  };

/*/////////////////////////
  FUNCTION DECLARATIONS
/////////////////////////*/

// This functions solves a matrix equation Ax=B (Gv=i)
// It takes any-sized float matrix A and B as an input and computes x.
MatrixXf solve_matrix_equation(MatrixXf A, MatrixXf B);

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

#endif

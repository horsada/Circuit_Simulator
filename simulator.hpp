#ifndef spice_hpp
#define spice_hpp

#include "dependencies.hpp"

/*///////////////////////
  CORE DATA STRUCTURE
///////////////////////*/

// Forward declarations
class node;
class component;

struct network {
  vector<component> network_components;
  vector<node> network_nodes;
};

struct node {
	string node_index;
	vector<component> connected_components;
};

class component {
  public:
    string component_name;
    vector<node> connected_terminals;

  component(string netlist_line);
};


class RCL: public component
{
	float component_value;
};


class independent_source 
	:public component
{
	string waveform_type; //ac or dc (we might need to add member variables)
	vector<float> values; // contains all 
	float output_values;
};

class current_dependent_source
  :public component
  {
    component current_controller;
    float constant_factor;
  };

class voltage_dependent_source
  :public component
  {
    vector<node *> control_nodes;
    float constant_factor;
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

// This function takes in a string suffix value (7.2k, 25m, 15M) and converts to a double.
double suffix_parser(string prefix_value);

#endif
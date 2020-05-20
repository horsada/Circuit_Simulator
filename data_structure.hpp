#ifndef spice_hpp
#define spice_hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <complex>

using namespace std;

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

  virtual component(string netlist_line);
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

#endif
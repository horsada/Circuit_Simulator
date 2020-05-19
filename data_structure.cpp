#ifndef "spice_hpp"
#define "spice_hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <complex>

struct node {
	string node_index;
	vector<component> connected_components;
}

class component {

}

struct RCL
	:public component
{
	string component_type;
	int value; // should be 0 for RCL, but dc or ac for voltage sources, or Opamp names?
	float component_value;
	node node1; // positive terminal
	node node2; // negative terminal
}

struct independent_source 
	:public component
{
	string source_type; // V source or I source
	string waveform_type; // set to 0 for independent source
	vector<float> values; // contains all 
	float output_values;
	node node1;  // positive terminal
	node node2;  // negative terminal
}

struct dependent_source 
	:public component
{
	string source_type; // VCVS, VCIS, ICVS, ICIS
	component current_controller;
	node node1;  // positive terminal
	node node2;  // negative terminal
	node ctrl_node1;  // positive terminal
	node ctrl_node2;  // negative terminal
}
#include "simulator.hpp"
#include "dependencies.hpp"

// Returns status code of parse operation: 0-success; 1-end_of_file; 2-parser_error;
int parse_netlist_line(network_simulation &netlist_network, string netlist_line) {
  // REGEX is used to verify and classify the netlist line
  // There are different types of lines in reduced spice format
  //1:Component => <designator> <node0> <node1> [<node 2] <value>
  regex reduced_spice_format_component("(V|I|R|C|L|D|Q)[0-9]+ N?[0-9]+ N?[0-9]+ (N?[0-9]+ )?.+");
  //2:Comment => *XXXXX
  regex reduced_spice_format_comment("\\*.+");
  //3:Transient simulation paramters => .tran 0 <stop time> 0 <timestep>
  regex reduced_spice_format_tran(".tran 0 [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)?s 0 [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)?s");
  //4:End of spice netlist => .end
  regex reduced_spice_format_end(".end");

  // Trying to match one of the three types
  if (regex_match(netlist_line, reduced_spice_format_component)) {
    // Line is a component

    // Similar parsing process for R, C, L
    if(netlist_line[0]=='R'||netlist_line[0]=='C'||netlist_line[0]=='L') {
      string component_name, node1_raw, node2_raw, value_with_suffix;
      int node_index_1, node_index_2;
      double component_value;

      // String stream to separate netlist line
      stringstream input(netlist_line);
      input >> component_name >> node1_raw >> node2_raw >> value_with_suffix;

      // Converting raw netlist components to usable values
      component_value = suffix_parser(value_with_suffix);
      node_index_1 = parse_node_name_to_index(node1_raw);
      node_index_2 = parse_node_name_to_index(node2_raw);

      // defining & pushing nodes, if not existing
      node new_node_1(node_index_1);
      node new_node_2(node_index_2);
      vector<node> new_nodes = {new_node_1, new_node_2};
      push_nodes(netlist_network, new_nodes);

      // Resistor
      if(netlist_line[0]=='R') {
        // defining and pushing new component
        R new_resistor(component_name, component_value, new_nodes);
        netlist_network.network_components.push_back(new_resistor);
      }
      // Capacitor
      if(netlist_line[0]=='C') {
        // defining and pushing new component
        C new_capacitor(component_name, component_value, new_nodes);
        netlist_network.network_components.push_back(new_capacitor);
      }
      // Inductor
      if(netlist_line[0]=='L') {
        // defining and pushing new component
        L new_inductor(component_name, component_value, new_nodes);
        netlist_network.network_components.push_back(new_inductor);
      }
    }

    // AC Sources
    // Matches SINE function
    regex full_sine_function_pattern("(V|I).+SINE\\([0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)? [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)? [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)?\\)");
    smatch sine_function_matches_exists;

    if(regex_search(netlist_line, sine_function_matches_exists, full_sine_function_pattern)) {
      // Extracting basic netlist line sections
      string component_name, node1_raw, node2_raw;
      int node_index_1, node_index_2;
      stringstream input1(netlist_line);
      input1 >> component_name >> node1_raw >> node2_raw;

      // Converting raw netlist components to usable values
      node_index_1 = parse_node_name_to_index(node1_raw);
      node_index_2 = parse_node_name_to_index(node2_raw);

      // defining & pushing nodes, if not existing
      node new_node_1(node_index_1);
      node new_node_2(node_index_2);
      vector<node> new_nodes = {new_node_1, new_node_2};
      push_nodes(netlist_network, new_nodes);

      // Extract SINE(X Y Z) function parameters
      regex sine_paramters_pattern("[0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)? [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)? [0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)?");
      smatch sine_function_paramters_only;
      regex_search(sine_function_matches_exists.str(0), sine_function_paramters_only, sine_paramters_pattern);

      string dc_offset_raw, amplitude_raw, frequency_raw;
      double dc_offset, amplitude, frequency;

      stringstream input(sine_function_paramters_only.str(0));
      input >> dc_offset_raw >> amplitude_raw >> frequency_raw;

      dc_offset = suffix_parser(dc_offset_raw);
      amplitude = suffix_parser(amplitude_raw);
      frequency = suffix_parser(frequency_raw);

      // AC voltage source
      if(netlist_line[0]=='V') {
        independent_v_source new_v_source(component_name, dc_offset, amplitude, frequency, new_nodes);
        netlist_network.network_components.push_back(new_v_source);
      }

      // AC current sources
      if(netlist_line[0]=='I') {
        independent_i_source new_i_source(component_name, dc_offset, amplitude, frequency, new_nodes);
        netlist_network.network_components.push_back(new_i_source);
      }
    } else if(netlist_line[0]=='V' || netlist_line[0]=='I') {
      // Extracting basic netlist line sections
      string component_name, node1_raw, node2_raw, dc_value_raw;
      int node_index_1, node_index_2;
      stringstream input1(netlist_line);
      input1 >> component_name >> node1_raw >> node2_raw >> dc_value_raw;

      // Converting raw netlist components to usable values
      node_index_1 = parse_node_name_to_index(node1_raw);
      node_index_2 = parse_node_name_to_index(node2_raw);

      // defining & pushing nodes, if not existing
      node new_node_1(node_index_1);
      node new_node_2(node_index_2);
      vector<node> new_nodes = {new_node_1, new_node_2};
      push_nodes(netlist_network, new_nodes);

      double dc_offset, amplitude, frequency;
      dc_offset = suffix_parser(dc_value_raw);
      amplitude = 0.0;
      frequency = 0.0;

      // AC voltage source
      if(netlist_line[0]=='V') {
        independent_v_source new_v_source(component_name, dc_offset, amplitude, frequency, new_nodes);
        netlist_network.network_components.push_back(new_v_source);
      }

      // AC current sources
      if(netlist_line[0]=='I') {
        independent_i_source new_i_source(component_name, dc_offset, amplitude, frequency, new_nodes);
        netlist_network.network_components.push_back(new_i_source);
      }

    }

    // Current sources DC

    // Current source AC
    // Voltage sources DC
    // Voltage source AC

    // Diode
    // Transistor
    return 0;
  }
  else if (regex_match(netlist_line, reduced_spice_format_comment)) {
    // Line is a comment, ignored
    return 0;
  }
  else if (regex_match(netlist_line, reduced_spice_format_tran)) {

    string netlist_stop_time_with_suffix, netlist_timestep_with_suffix, placeholder;
    double netlist_stop_time, netlist_timestep;

    // As the line is space-separated, a string stream is used to separate out the values
    stringstream input(netlist_line);
    input >> placeholder >> placeholder >> netlist_stop_time_with_suffix >> placeholder >> netlist_timestep_with_suffix;

    netlist_stop_time = suffix_parser(netlist_stop_time_with_suffix);
    netlist_timestep = suffix_parser(netlist_timestep_with_suffix);

    // Set network parameters
    netlist_network.stop_time = netlist_stop_time;
    netlist_network.timestep = netlist_timestep;

    return 0;
  }
  else if (regex_match(netlist_line, reduced_spice_format_end)) {
    // Line is a .end, ignored
    return 1; // End of netlist reached
  }
  else {
    return 2; // Error: Invalid netlist format
  }
}

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
    // Classify component, get nodes, get component; push to netlist_network
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
    netlist_network.stop_time=netlist_stop_time;
    netlist_network.timestep=netlist_timestep;
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

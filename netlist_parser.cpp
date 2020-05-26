#include "simulator.hpp"
#include "dependencies.hpp"

// Returns status code of parse operation: 0-success; 1-end_of_file; 2-parser_error;
int parse_netlist_line(network_simulation netlist_network, string netlist_line) {
  cout << "netlist_parser_here" << endl;
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
    cout << "end of netlist" << endl;

    return 0;
  }
  else if (regex_match(netlist_line, reduced_spice_format_comment)) {
    // Line is a comment, ignored
    return 0;
  }
  else if (regex_match(netlist_line, reduced_spice_format_tran)) {
    // Line is a .tran
    double netlist_stop_time, netlist_timestep;
    smatch matches;
    regex time_values(" 0 (.*?)s"); // Matches e.g. " 0 10ms", need substrings (3, end-1)
    regex_search(netlist_line, matches, time_values);
    assert(matches.size()==2); // There should be two matches in the line
    //netlist_stop_time = suffix_parser(matches[0].substr(3, matches[0].length()-4));
    //netlist_timestep = suffix_parser(matches[1].substr(3, matches[1].length()-4));
    cout << "netlist_stop_time: " << netlist_stop_time << endl;
    cout << "netlist_timestep: " << netlist_timestep << endl;
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

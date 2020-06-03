#include "simulator.hpp"
#include "dependencies.hpp"

MatrixXf solve_matrix_equation(MatrixXf A, MatrixXf B) {
  VectorXf x = A.colPivHouseholderQr().solve(B);
  return x;
}

// Testing needed
double suffix_parser(string input) {
  // reduced spice format takes multiplier or normal float as input

  // This strips the unit from back, if it is present (5ms=>5m)
  regex has_unit(".+(s|Ohm|Ω|F|H)");
  if (regex_match(input, has_unit)) {
    input = input.substr(0, input.size()-1);
  }

  // Input format examples: 1m, 0.1 ...
  // 1. Check if input already is a number
  regex pure_number("[0-9]+([.][0-9]+)?");
  if (regex_match(input, pure_number)) {
    return stod(input);
  }

  // 2. Check for metric suffix
  regex metric_suffix("[0-9]+([.][0-9]+)?(p|n|u|m|k|Meg|G)");
  if (regex_match(input, metric_suffix)) {
    if(input.find("p") != string::npos) { return stod(input.substr(0, input.size()-1))*1e-12; }
    if(input.find("n") != string::npos) { return stod(input.substr(0, input.size()-1))*1e-9; }
    if(input.find("u") != string::npos) { return stod(input.substr(0, input.size()-1))*1e-6; }
    if(input.find("m") != string::npos) { return stod(input.substr(0, input.size()-1))*1e-3; }
    if(input.find("k") != string::npos) { return stod(input.substr(0, input.size()-1))*1e3; }
    if(input.find("Meg") != string::npos) { return stod(input.substr(0, input.size()-3))*1e6; }
    if(input.find("G") != string::npos) { return stod(input.substr(0, input.size()-1))*1e9; }
  }

  // No matching case.
  assert(1 && "Invalid number formatting in netlist."); // Invalid input format
  return 0; // To avoid compiler errors
}

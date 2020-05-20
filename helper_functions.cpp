#include "simulator.hpp"
#include "dependencies.hpp"

MatrixXf solve_matrix_equation(MatrixXf A, MatrixXf B) {
  VectorXf x = A.colPivHouseholderQr().solve(B);
  return x;
}

double suffix_parser(string input) {
  // Categorising the format of the suffix. (1000, K, E+03, Kilo)

  // 1. Check if input already is a number
  regex formatted_number("[0-9]+([.][0-9]+)?");
  if (regex_match(input, formatted_number)) {
    return stod(input);
  }

  // 2.check for regular metric suffix
  regex metric_suffix("[0-9]+([.][0-9]+)?[TGXKMUNPF]");
  char last_char = input.back();
  /*
  if (regex_match(input, formatted_number)) {
    if(last_char=="T") {
      return stod(input.substr(0, input.size()-1))*100;
    }
    else if(last_char=="T") {
      
    }
    else if(last_char=="G") {
      
    }
    else if(last_char=="X") {
      
    }
    else if(last_char=="K") {
      
    }
    else if(last_char=="M") {
      
    }
    else if(last_char=="U") {
      
    }
    else if(last_char=="N") {
      
    }
    else if(last_char=="P") {
      
    }
    else if(last_char=="F") {
      
    }
  }
  */
}
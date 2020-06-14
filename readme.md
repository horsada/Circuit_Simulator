

## **How to compile the simulator**

 - To compile, C++11 (or higher) is needed.
 - The external Eigen library directory must be included.

**To install the Eigen library, clone it into the repositories root directory:**

    git clone https://gitlab.com/libeigen/eigen.git

**Compilation command:**

	g++ -I eigen/ -std=c++11 matrix_helpers.cpp matrix_factory.cpp netlist_parser_helpers.cpp netlist_parser.cpp write_outputs_in_CSV.cpp -o current_test

For every compilation, name the output file extension .out, to ensure they are ignored by source control.

The input netlist file is called 
	
	netlist.txt

The output csv file is called 

	output.csv

After the derired circuit is written in netlist.txt, run ./current_test to write the outputs into output.csv.

For calculating the inverse of a matrix by using Eigen library, do

	G = G.inverse(); // assuming A to be a declared matrix

For Matrix-matrix multiplication, do

	G = G.inverse();
	V = G * I;

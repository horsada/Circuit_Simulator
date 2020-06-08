

## **How to compile the simulator**

 - To compile, C++11 (or higher) is needed.
 - The external Eigen library directory must be included.

**To install the Eigen library, clone it into the repositories root directory:**

      git clone https://gitlab.com/libeigen/eigen.git

**Example compilation (add files as necessary):**

    g++ -I eigen/ -std=c++11 helper_functions.cpp test_sandbox.cpp netlist_parser.cpp -o compiled_test

For every compilation, name the output file extension .out, to ensure they are ignored by source control.

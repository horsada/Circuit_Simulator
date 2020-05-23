// This is a compilable test-sandbox.
// New functions can be compiled and tested on the fly here.
#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

int main()
{
    //cout << suffix_parser("") << endl;
    network_simulation sim;
    cout << parse_netlist_line(sim, ".tran 0 10ms 0 1us");
}

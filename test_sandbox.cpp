// This is a compilable test-sandbox.
// New functions can be compiled and tested on the fly here.
#include "simulator.hpp"
#include "dependencies.hpp"

using namespace std;
using namespace Eigen;

int main()
{
   Matrix<float, 2, 2> A;
   Matrix<float, 2, 1> b;
   A << 1,2,  4,5;
   b << 3, 3;
   cout << solve_matrix_equation(A, b) << endl;
   cout << 1023.23 * e5;
}

#include "dependencies.hpp"
#include "simulator.hpp"

complex<float> get_impedance(R r, float angular_fre){
	return r.read_value();
}

// complex<float> get_impedance(C c, float angular_fre){
// 	float interm;
// 	interm = c.read_value();
// 	return {0, -1/(angular_fre*interm)};
// }
//
// complex<float> get_impedance(L l, float angular_fre){
// 	float interm;
// 	interm = l.read_value();
// 	return {0, angular_fre*interm};
// }
//
// complex<float> get_impedance(diode d, float angular_fre){}

//diode is leftout for now. I think it can be treated as a current source with an extra switch, which is controlled by the voltage difference across the diode

/*
Something to achieve the current goal point
*/

#include <vector>
#include "PDcontroller.h"


using namespace std;
// PDcontroller::PDcontroller() {

// }

// PDcontroller::~PDcontroller() {

// }

float PDcontroller::getControlInput(vector<float> xdes_in, vector<float> xcur_in, float dt) {
	// calculate errors
	xcur = xcur_in;
	xdes = xdes_in;
	e_last = e;
	e[0] = xdes[0] - xcur[0];
	e[1] = xdes[1] - xcur[1];
	float u = Kp*e[0] + Kd*e[1];
	return u;
}




// simulate a 1D integrator trying to achieve a goal position



/* Steps
1) Dynamics
2) Output text file
3) Plot in Matlab
4) build a trajectory
5) Implement a controller -- use inheritance
*/


#include "dynamics.h"
#include "PDcontroller.h"
#include <iostream>
#include <cmath>
#include <vector>

int main(){
	float dt = 0.001;
	Integrator_1D D(dt);
	D.printName();
	D.printPosition();

	std::vector<float> xdes = {1,0};

	PDcontroller PD;
	float u;

	while (std::abs(PD.e[0])>0.001) {
		u = PD.getControlInput(xdes, D.state, dt);
		std::cout << "Input: " << u << ", Error: " << PD.e[0] << std::endl;
		// D.step(u);
		D.noisyStep(u);
		D.recordState();
		D.writeState();
		D.writeNoisyState();
		// std::vector<float> noisy = D.getNoisyState();
	}
	D.closeFile();
	std::cout << D.state_all.size() << std::endl;
	// for (std::vector<T>::iterator i=D.state_all.begin(); i<D.state_all.end(); i++)  {
	for (auto const& i: D.state_all)  {
		// D.printState(i);
	}
	D.printName();

}

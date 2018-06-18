
#include <vector>
#include <string>
#include "dynamics.h"
#include <iostream>
#include <fstream> 
#include <random>

using namespace std;
Integrator_1D::Integrator_1D(float dt_in) {
	// constructor
	dt = dt_in;
	fn_name = "State_recording.csv";
	noisy_fn_name = "NoisyState_recording.csv";
	state_file.open(fn_name,std::ofstream::out);
	noisy_state_file.open(noisy_fn_name,std::ofstream::out);
	default_random_engine generator;
	// random_device rd;
	normal_distribution<float> distribution(0.0,1.0);
	// mt19937 gen(time(0));
	// for (int i = 0; i < 100; i++) {
	// 	float noise_added = distribution(generator);
	// 	cout << "Noise Added: " << noise_added << endl;
	// }
}


void Integrator_1D::printName() {
	cout << "Name: " << name << endl;
}

void Integrator_1D::printPosition() {
	cout << "Position: " << state[0] << endl;
}
void Integrator_1D::printVelocity() {
	cout << "Velocity: " << state[1] << endl;
}
void Integrator_1D::printState() {
	cout << "Position: " << state[0] << ", Velocity: " << state[1] << endl;
}
void Integrator_1D::printState(int i) {
	cout << "Position: " << state_all[i][0] << ", Velocity: " << state_all[i][1] << endl;
}
void Integrator_1D::printState(vector<float> state_in) {
	cout << "Position: " << state_in[0] << ", Velocity: " << state_in[1] << endl;
}

void Integrator_1D::step(float u_in) {
	u = u_in;
	state[2] = u;
	state[1] += dt*u;
	state[0] += dt*state[1];
}

void Integrator_1D::noisyStep(float u_in) {
	u = u_in;
	// float noise_added = d(generator);
	normal_distribution<float> distribution(0.0,500.0); // doesn't work if its not in the function!
	float noise_added = distribution(generator);
	// cout << "Mean: " << distribution.mean() << ", StDev: " << distribution.stddev() << endl;
	u_noise = u_in + noise_added;
	cout << "Noise Added: " << noise_added << ", ";
	state[2] = u_noise;
	state[1] += dt*state[2];
	state[0] += dt*state[1];
}

void Integrator_1D::setdt(float dt_in) {
	dt = dt_in;
}


float Integrator_1D::getdt() {
	cout << "Dynamics Time: " << dt << endl;
	return dt;
}

void Integrator_1D::setState(vector<float> state_in){
	state = state_in;
}

void Integrator_1D::recordState(){
	state_all.push_back(state);
}

void Integrator_1D::writeState(){
	// state_file.open(fn_name);
	state_file << dt << ", ";
	state_file << state[0] << ", ";
	state_file << state[1] << ", ";
	state_file << u;
	state_file << endl;
	// state_file.open(fn_name);
}

void Integrator_1D::writeNoisyState(){
	vector<float> state_noisy = getNoisyState();
	noisy_state_file << dt << ", ";
	noisy_state_file << state_noisy[0] << ", ";
	noisy_state_file << state_noisy[1] << ", ";
	noisy_state_file << u_noise;
	noisy_state_file << endl;
	// state_file.open(fn_name);
}

void Integrator_1D::closeFile() {
	state_file.close();
	noisy_state_file.close();
}

vector<float> Integrator_1D::getState() {
	return state;
}

vector<float> Integrator_1D::getNoisyState(){
	vector<float> state_noisy;
	normal_distribution<float> distribution(0.0,1.0); // doesn't work if its not in the function!
	float noise_added = distribution(generator);
	int c = 0;
	for (auto const& i: state)  {
		state_noisy.push_back(i + noise_added);
	}
	return state_noisy;

}
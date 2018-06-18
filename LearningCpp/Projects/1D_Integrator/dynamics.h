
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

class Integrator_1D{
public:
	std::string name = "Ammar!";
	std::vector<float> state = {0,0,0};
	std::string fn_name;
	std::string noisy_fn_name;
	std::ofstream state_file;
	std::ofstream noisy_state_file;
	float mass;
	std::vector< std::vector<float> > state_all;
	std::default_random_engine generator;
	std::normal_distribution<float> distribution;
	// std::mt19937 gen;
	float u_noise;
	float u;


	Integrator_1D(float);
	void setdt(float);
	float getdt();
	void printName();
	void printPosition();
	void printVelocity();
	void printState();
	void printState(int);
	void printState(std::vector<float>);
	void recordState();
	void writeState();
	void writeNoisyState();
	void step(float);
	void noisyStep(float);
	void setState(std::vector<float>);
	void closeFile();

	std::vector<float> getState();
	std::vector<float> getNoisyState();

private:
	float dt;

};
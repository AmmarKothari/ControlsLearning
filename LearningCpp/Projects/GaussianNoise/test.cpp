#include <iostream>

#include <random>


class make_noise{
public:
	std::default_random_engine generator;
	std::normal_distribution<float> distribution;
	make_noise() {
		static std::default_random_engine generator;
		static std::normal_distribution<float> distribution(0.0,112.0);
	}
	void printNoise() {
		float number = distribution(generator);
		std::cout << "Number: " << number << std::endl;

	}
private:
};


int main() {
	make_noise N;

	for (int i = 0; i < 100; i++) {
		N.printNoise();
	}
	return 1;
}
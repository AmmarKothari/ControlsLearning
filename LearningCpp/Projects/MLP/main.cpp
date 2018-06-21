


#include <stdlib.h>
#include <vector>
#include <iostream>
#include "MLP.h"


int main() {
	
	// Generate a dataset
	srand(0);
	int data_points = 1000;
	std::vector< std::vector<int> > data;
	std::vector<int> data1 = {0,0};
	std::vector<int> output;

	for (int i=0; i<data_points; i++) {
		data1[0] = rand() % 2; // values either 0 and 1
		data1[1] = rand() % 2;
		data.push_back(data1);
	}

	// // And Data set
	for (auto const& i: data) {
		if (i[0] + i[1] == 2) {
			output.push_back(1);
			
		} else {
			output.push_back(0);
		}
	}

	// print dataset
	// int c = 0;
	// for (auto const& i: data) {
	// 	std::cout << "In: " << i[0] << ", " << i[1] << " \t Out: " << output[c] << std::endl;
	// 	c++;
	// }
	MLP mlp(2, 10, 1);
	for (int i = 0; i < 1; i++) {
		// std::cout << "Run " << i << std::endl;
		
		// mlp.printLayers();
		mlp.train(10000, 100, data, output);
	}





	// int input_dims = 10;



	return -1;
}
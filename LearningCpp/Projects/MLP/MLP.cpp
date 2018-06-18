
#include <iostream>
#include <random>
#include <vector>
#include "MLP.h"


using namespace std;
MLP::MLP(int input_dims_in, int hidden_nodes_in, int bias_in) {
	input_dims_ct = input_dims_in;
	hidden_nodes_ct = hidden_nodes_in;
	bias_val = bias_in;
	output_dim_ct = 1;

	float init_c = 1;
	float init_spread = 2;

	// layers
	default_random_engine generator;
	uniform_real_distribution<float> distribution(init_c,init_spread);

	// initialize layers with random values
	vector<float> temp;
	for (int i1 = 0; i1 < hidden_nodes_ct; i1++) {
		for (int i2 = 0; i2 < input_dims_ct+1; i2++) {
			temp.push_back(distribution(generator));
		}
		W1.push_back(temp);
		temp.clear();
	}
	for (int i1 = 0; i1 < hidden_nodes_ct+1; i1++) {
		for (int i2 = 0; i2 < output_dim_ct; i2++) {
			temp.push_back(distribution(generator));
		}
		W2.push_back(temp);
		temp.clear();
	}
}


void MLP::printLayers() {
	cout << "W1" << endl;
	for (int i1 = 0; i1 < hidden_nodes_ct; i1++) {
		for (int i2 = 0; i2 < input_dims_ct+1; i2++) {
			cout << W1[i1][i2] << "  ";
		}
		cout << endl;
	}
	cout << "W2" << endl;
	for (int i1 = 0; i1 < hidden_nodes_ct+1; i1++) {
		for (int i2 = 0; i2 < output_dim_ct; i2++) {
			cout << W2[i1][i2] << "  ";
		}
		cout << endl;
	}
}

void MLP::train(int iters, int batch_size, vector <vector<int> > &data) {
	// if (batch_size > (int)data.size()){
	// 	batch_size = data.size();
	// }
	cout << "Batch Size: " << batch_size << endl;
	cout << "Iterations: " << iters << endl;
	// cout << "Samples: " << data.size() << endl;
	// iteration loop
	for (int it = 0; it<iters; it++) {
		for (int ib = 0; ib<batch_size; ib++) {
			cout << " "; // work on this section!
		}

	}
}
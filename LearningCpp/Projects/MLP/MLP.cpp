
#include <iostream>
#include <random>
#include <vector>
#include <math.h>

#include "MLP.h"


#define DEBUG1() cout<<"Debug 1"<<endl;
#define DEBUG2() cout<<"Debug 2"<<endl;
#define DEBUG3() cout<<"Debug 3"<<endl;
#define DEBUG(X) cout<<"Debug "<<X<<endl;


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
	printMatrix(W1);
	cout << "W2" << endl;
	printMatrix(W2);
	// for (int i1 = 0; i1 < hidden_nodes_ct+1; i1++) {
	// 	for (int i2 = 0; i2 < output_dim_ct; i2++) {
	// 		cout << W2[i1][i2] << "  ";
	// 	}
	// 	cout << endl;
	// }
}

void MLP::printMatrix(vector< vector<float> > &mat_in) {
	for (auto const& row: mat_in) {
		for (auto const& col: row) {
			cout << col << "  ";
		}
		cout << endl;
	}
}


float MLP::sigmoid(float x)
{
     float exp_value;
     float return_value;

     /*** Exponential calculation ***/
     // cout << "Sigmoid In: " << x;
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1.0 / (1.0 + exp_value);
     // cout << "  Sigmoid Out: " << return_value << endl;
     return return_value;
}

vector<float> MLP::add_bias_node(vector<float> &in_nodes){
	vector<float> full_nodes = in_nodes;
	full_nodes.push_back(bias_val);
	return full_nodes;
}


vector<float> MLP::add_bias_node(vector<int> &in_nodes){
	vector<float> floatVec(in_nodes.begin(), in_nodes.end());
	return add_bias_node(floatVec);
}

vector<float> MLP::forward_1Layer(vector<float> &in_nodes, vector< vector<float> > &weights_in) {
	// run forward through a single layer
	// vector<float> sample(in_nodes.begin(), in_nodes.end());
	// sample.push_back(bias_val); // add the bias unit
	vector<float> sample;
	sample = add_bias_node(in_nodes);
	// cout << "Sample: ";
	// for (auto const& s: sample) {
	// 	cout << s << "  ";
	// }
	// cout << endl;
	float node_val = 0; // forward prop node value
	float squashed_node_val = 0; // flattened value
	vector<float> NL; // Output Layer values
	// DEBUG1();
	for (auto const& W_row: weights_in) {
		// weights_row * sample_col
		int i_w = 0;
		// cout << "Weights: ";
			for (auto const& s: sample)  {
				// move the sample forward through the network
				// cout << W_row[i_w] << "   ";
				node_val +=  W_row[i_w]*s;
				i_w++;
			}
		squashed_node_val = sigmoid(node_val);
		NL.push_back(squashed_node_val);
		// cout << "Node Value: " << node_val << endl;
		node_val = 0;
		squashed_node_val = 0;
	}
	return NL;
}


vector<float> MLP::forward_1Layer(vector<int> &in_nodes, vector< vector<float> > &weights_in) {
	// convert input to float and then run forward
	vector<float> floatVec(in_nodes.begin(), in_nodes.end());
	return forward_1Layer(floatVec, weights_in);
}

vector<int> MLP::classify_LastLayer(vector<float> &in_nodes) {
	vector<int> out;
	for (auto const& v: in_nodes){
		if (v < 0.5) {
			out.push_back(0);
		} else {
			out.push_back(1);
		}
	}
	return out;
}

float MLP::loss(int &actual, float &pred_NL) {
	float L;
	// cout << pred_NL << " ";
	// cout << log(pred_NL) << " ";
	// cout << log(1.0 - pred_NL) << " ";
	L = ( (float)actual - log(pred_NL) ) + ( (1.0 - (float)actual) * log(1.0 - pred_NL) );
	// cout << L << endl;
	L = max((float)-1e6, min((float)1e6, L));
	return L;
}

void MLP::initializeMat(vector< vector<float> > & mat_in, int rows, int cols) {
	vector<float> temp;
	for (size_t i1 = 0; i1 < rows; i1++) {
		temp.clear();
		for (size_t i2 = 0; i2 < cols; i2++) {
			temp.push_back(0);
		}
		mat_in.push_back(temp);
	}
}

void MLP::train(int iters, int batch_size, vector <vector<int> > &data, vector<int> &results) {
	if (batch_size > (int)data.size()){
		batch_size = data.size();
	}
	cout << "Batch Size: " << batch_size << endl;
	cout << "Iterations: " << iters << endl;
	// cout << "Samples: " << data.size() << endl;
	default_random_engine generator;
	// iteration loop
	for (int it = 0; it<iters; it++) {
		cout << "Iteration: " << it << endl;
		/////////////////FORWARD//////////////////////
		vector< vector<float> > dL_w2;
		// initializeMat(dL_w2, hidden_nodes_ct, output_dim_ct);
		initializeMat(dL_w2, 10, 10);
		// vector<float> dL_w1;
		float L = 0;
		float delta_i = 0;
		for (size_t ib = 0; ib<batch_size; ib++) {
			int sample_idx;
			uniform_int_distribution<int> batch_sample_distribution(0,data.size());
			sample_idx = batch_sample_distribution(generator);
			vector<int> sample = data[sample_idx]; 
			/* 
			LAYER 1
			*/
			vector<float> NL1;
			NL1 = forward_1Layer(sample, W1);

			/* 
			LAYER 2
			*/
			// cout << "Layer 2" << endl;
			vector<float> NL2; // single value
			NL2 = forward_1Layer(NL1, W2);
			// cout << "NL: " << NL2[0] << endl;

			/*
			Classify
			*/
			vector<int> out;  // should be a single value
			out = classify_LastLayer(NL2);

			// Loss
			L -= loss(results[sample_idx], NL2[0]);
			cout << "Output: " << out[0] << ", Actual: " << results[sample_idx] << ", Loss: " << L << endl;

			delta_i = results[sample_idx] - out[0];
			vector<float> NL1B;
			NL1B = add_bias_node(NL1);
			for (size_t i_d=0; i_d < NL1B.size(); i_d++){
				for (size_t i_w=0; i_w<dL_w2[i_d].size(); i_w++) {
				// for (auto const& n: NL1B) {
					// cout << i_d << " " << dL_w2[i_d][i_w];
					 // << " " << dL_w2[i_d] << " " << delta_i << " " << n;
					dL_w2[i_d][i_w] += delta_i*NL1B[i_d];
					// DEBUG1();
				}
			}
			// printMatrix(dL_w2);
			vector<float> sampleB;
			sampleB = add_bias_node(sample);
			for (size_t i_d = 0; i_d<NL1.size()-1; i_d++) {
				float dNL1_dW = NL1[i_d]*(1-NL1[i_d]);
			// 	float dW2_dy = 0;
			// 	for (size_t i_w=0; i_w < W2[i_d].size(); i_w++) {
			// 		dW2_dy += W2[i_d][i_w]*(results[sample_idx] - NL2[0]);
				// }
			// 	dL_w1[i_d] += ( dNL1_dW * dW2_dy) * x_b(i1,:);
			}
		}
		// del dL_w2;
		cout << "End Batch: " << it << endl;
		cout << endl;

		/////////////////BACKWARD//////////////////////

	}
}
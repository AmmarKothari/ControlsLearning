
#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

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

	learning_rate = 0.5;

	// layers
	default_random_engine generator(100);
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

void MLP::printMatrix(const vector< vector<float> > &mat_in) {
	for (auto const& row: mat_in) {
		printMatrix(row);
	}
}

void MLP::printMatrix(const vector<float> &vec_in) {
	for (auto const& col: vec_in) {
		cout << col << "  ";
	}
	cout << endl;
}


float MLP::sigmoid(float x){
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
	full_nodes.push_back((float)bias_val);
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

void MLP::openCSVFile(ofstream &record, string fn){
	record.open(fn, ofstream::out);
}

void MLP::closeCSVFile(ofstream &record){
	record.close();
}

void MLP::train(int iters, int batch_size, vector <vector<int> > &data, vector<int> &results) {
	string Folder = "Records/";
	if (batch_size > (int)data.size()){
		batch_size = data.size();
	}
	cout << "Batch Size: " << batch_size << endl;
	cout << "Iterations: " << iters << endl;
	// cout << "Samples: " << data.size() << endl;
	default_random_engine generator;
	vector< vector<float> > dL_w2;
	vector< vector<float> > dL_w1;
	// iteration loop
	stringstream overall_fn;
	overall_fn << "Overall" << ".csv";
	ofstream batch_recorder;
	ofstream overall_recorder;
	openCSVFile(overall_recorder, overall_fn.str());
	printMatrix(W2);
	for (int it = 0; it<iters; it++) {
		cout << "Iteration: " << it << endl;
		stringstream batch_fn;
		batch_fn << Folder << "Batch_" << it << ".csv";

		
		openCSVFile(batch_recorder, batch_fn.str());
		
		/////////////////FORWARD//////////////////////
		dL_w2.clear();
		initializeMat(dL_w2, hidden_nodes_ct+1, output_dim_ct);
		dL_w1.clear();
		initializeMat(dL_w1, hidden_nodes_ct, input_dims_ct+1);
		float L = 0;
		float delta_i = 0;
		vector<float> NL1;
		vector<float> NL1B;
		vector<float> NL2; // single value
		int acc = 0; // number of correct preditions
		for (size_t ib = 0; ib<batch_size; ib++) {
			int sample_idx;
			uniform_int_distribution<int> batch_sample_distribution(0,data.size());
			sample_idx = batch_sample_distribution(generator);
			vector<int> sample = data[sample_idx]; 
			/* 
			LAYER 1
			*/
			NL1.clear();
			NL1 = forward_1Layer(sample, W1);

			/* 
			LAYER 2
			*/
			// cout << "Layer 2" << endl;
			NL2.clear();
			NL2 = forward_1Layer(NL1, W2);
			// cout << "NL: " << NL2[0] << endl;

			/*
			Classify
			*/
			vector<int> out;  // should be a single value
			out = classify_LastLayer(NL2);

			if (out[0] == results[sample_idx]) {
				acc++;
			}

			// Loss
			L -= loss(results[sample_idx], NL2[0]);
			// cout << "Output: " << out[0] << ", Actual: " << results[sample_idx] << ", Loss: " << L << endl;

			delta_i = (float) ((float)results[sample_idx] - NL2[0]);
			NL1B.clear();
			NL1B = add_bias_node(NL1);
			if (isnan(NL1B[0])) {
				cout << "Nan found" << endl;
				exit(EXIT_FAILURE);
			}
			// 
			// cout << delta_i << endl;
			// DEBUG1();
			for (size_t i_d=0; i_d < dL_w2.size(); i_d++){
				for (size_t i_w=0; i_w<dL_w2[i_d].size(); i_w++) {
				// for (auto const& n: NL1B) {
					// cout << i_d << " " << dL_w2[i_d][i_w] << "   ";
					 // << " " << dL_w2[i_d] << " " << delta_i << " " << n;
					dL_w2[i_d][i_w] += delta_i*NL1B[i_d];
					// cout << dL_w2[i_d][i_w] << endl;
					// DEBUG1();
				}
			}
			/////////////////BACKWARD//////////////////////
			vector<float> sampleB;
			sampleB = add_bias_node(sample);
			for (size_t i_d = 0; i_d<NL1.size()-1; i_d++) { // each hidden node
				float dNL1_dW = NL1[i_d]*(1-NL1[i_d]);
				float dW2_dy = 0;
				for (size_t i_w=0; i_w < W2[i_d].size(); i_w++) { // a column vector
					dW2_dy += W2[i_d][i_w]*(results[sample_idx] - NL2[0]);
				}
				for (size_t i_s = 0; i_s<sample.size(); i_s++) { // each input
					// for (size_t i_w=0; i_w<dL_w1[i_d].size(); i_d++) { // multiply by samples input to get weight matrix
					// 	dL_w1[i_d][i_w] += ( dNL1_dW * dW2_dy) * sampleB[i_d];
					// }
					dL_w1[i_d][i_s] += ( dNL1_dW * dW2_dy) * sampleB[i_s];
				}
			}

			// Write values to file
			batch_recorder << results[sample_idx] << ", " << out[0] << endl;
		}

		///////////// WRITE results to CSV File/////////////
		closeCSVFile(batch_recorder);
		overall_recorder << it << ", " << (float)acc/(float)batch_size << endl;

		/////////////////UPDATE STEP//////////////////////
		for (size_t i_r = 0; i_r<W1.size(); i_r++) {
			for (size_t i_c = 0; i_c<W1[i_r].size(); i_c++) {
				W1[i_r][i_c] += learning_rate * dL_w1[i_r][i_c] / (float)(batch_size);
			}
		}
		for (size_t i_r = 0; i_r<W2.size(); i_r++) {
			for (size_t i_c = 0; i_c<W2[i_r].size(); i_c++) {
				// cout << learning_rate * dL_w2[i_r][i_c] / (float)(input_dims_ct+1) << endl;
				// cout << learning_rate << endl;
				W2[i_r][i_c] += learning_rate * dL_w2[i_r][i_c] / (float)(batch_size);
				// W2[i_r][i_c] += 0.00001;
			}
		}
		// continue;
		cout << "W1" << endl;
		printMatrix(W1);
		// cout << "W2" << endl;
		// printMatrix(W2);
		// cout << "dL_w2" << endl;
		// printMatrix(dL_w2);
		cout << "Accuracy: " << acc << ", Samples: " << batch_size << endl;
		// del dL_w2;
		cout << "End Batch: " << it << endl;
		cout << endl;

	}
	closeCSVFile(overall_recorder);
}
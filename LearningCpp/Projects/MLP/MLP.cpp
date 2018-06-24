
#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>

#include "MLP.h"


#define DEBUG1() cout<<"Debug 1"<<endl;
#define DEBUG2() cout<<"Debug 2"<<endl;
#define DEBUG3() cout<<"Debug 3"<<endl;
#define DEBUG(X) cout<<"Debug "<<X<<endl;
#define DEBUGVAR(S, X) cout<<"Debug " << S << ": " << X << endl;
#define DEBUGEXIT() exit(EXIT_FAILURE);
#define PRINTMATRIX(X,M) cout << "Debug " << X << ": " << M << endl;


using namespace std;
MLP::MLP(int input_dims_in, int hidden_nodes_in, int bias_in) {
	input_dims_ct = input_dims_in;
	hidden_nodes_ct = hidden_nodes_in;
	bias_val = bias_in;
	output_dim_ct = 1;
	learning_rate = 2.5;

	// initialize layers with random values
	initializeMatRandom(W1, hidden_nodes_ct, input_dims_ct+1);
	initializeMatRandom(W2, output_dim_ct, hidden_nodes_ct+1);

	printLayers();
}

void MLP::printLayers() {
	cout << "W1" << endl;
	printMatrix(W1);
	cout << "W2" << endl;
	printMatrix(W2);
}

void MLP::printMatrix(const vector< vector<float> > &mat_in) {
	for (auto const& row: mat_in) {
		cout << "[ ";
		printMatrix(row);
		cout << " ]";
		cout << endl;
	}
}

void MLP::printMatrix(const vector<float> &vec_in) {
	for (auto const& v: vec_in) {
		cout << "[";
		cout << v << "]" << "  ";
	}
}

void MLP::printMatrix(const vector<int> &vec_in) {
	for (auto const& v: vec_in) {
		cout << v << "  ";
	}
	cout << endl;
}

float MLP::sigmoid(float x){
     float exp_value;
     float return_value;
     /*** Exponential calculation ***/
     exp_value = exp((double) -x);

     /*** Final sigmoid value ***/
     return_value = 1.0 / (1.0 + exp_value);
     return return_value;
}

vector< vector<float> > MLP::sigmoid(vector< vector<float> > &mat_in){
	vector< vector<float> > squashed_mat;
    vector<float> temp;
    for (auto & row: mat_in) {
    	temp = sigmoid(row);
    	squashed_mat.push_back(temp);
    	temp.clear();
    }
    return squashed_mat;
}

vector<float> MLP::sigmoid(vector<float> &vec_in){
	vector<float> squashed_vec;
    for (auto const& val: vec_in) {
		squashed_vec.push_back(sigmoid(val));
	}
    return squashed_vec;
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

vector< vector<float> > MLP::removeBiasWeights(vector< vector<float> > &mat_in) {
    // removes last row of vector
    vector< vector<float> > mat_out(mat_in.begin(), mat_in.end());
    if (mat_out.size() <= 1){
    	mat_out[0].pop_back();
    } else {
    	mat_out.pop_back();
    }
    return mat_out;
}

vector< vector<float> > MLP::addDimension(vector <float> &vec_in) {
	vector< vector<float> > mat_out;
	vector<float> temp;
	for (auto & val: vec_in){
		temp.push_back(val);
		mat_out.push_back(temp);
		temp.clear();
	}
	return mat_out;
}

vector<float> MLP::removeDimension(vector< vector<float> > &mat_in) {
	vector<float> vec_out;
	for (auto const& row: mat_in) {
		for (auto const& val: row) {
			vec_out.push_back(val);
		}
	}
	return vec_out;
}

vector< vector<float> > MLP::transpose(vector< vector<float> > &mat_in){
	vector< vector<float> > mat_out;
	vector<float> temp;
	// transposes a vector
	// cout << "Matrix In: " << endl;
	// printMatrix(mat_in);
	// cout << "Row Count: " << mat_in.size() << endl;
	// cout << "Col Count: " << mat_in[0].size() << endl;
	for (size_t i_cols = 0; i_cols<mat_in[0].size(); i_cols++) {
		temp.clear();
		for (auto & row: mat_in) {
			// cout<< "Row" << endl;
			// printMatrix(row);
			temp.push_back(row[i_cols]);
		}
		// cout << "Column: " << i_cols << endl;
		// printMatrix(temp);
		mat_out.push_back(temp);
	}
	// cout << "Matrix Out: " << endl;
	// printMatrix(mat_out);
	return mat_out;
}

vector< vector<float> > MLP::transpose(vector<float> &vec_in){
	vector< vector<float> > mat;
	mat = addDimension(vec_in);
	vector< vector<float> > mat_out;
	mat_out = transpose(mat);
	return mat_out;
}

vector< vector<float> > MLP::matmul(vector< vector<float> > &mat_L, vector< vector<float> > &mat_R) {
	// cout << "MatL cols: " << mat_L[0].size() << ", MatR rows: " << mat_R.size() << endl;
	// printMatrix(mat_L);
	// printMatrix(mat_R);

	assert(mat_L[0].size() == mat_R.size());

	vector< vector<float> > mat_out;
	vector<float> temp;
	float val = 0;
	for (size_t c_row = 0; c_row<mat_L.size(); c_row++) { // for each row in L matrix
		for (size_t c_col = 0; c_col<mat_R[0].size(); c_col++) { // for col in R matrix
			val = 0;
			for (size_t i_col = 0; i_col<mat_L[c_row].size(); i_col++) { // across columns of L matrix
				for (size_t i_row = 0; i_row<mat_R.size(); i_row++) { // down rows of R matrix
					val += mat_L[c_row][i_col] * mat_R[i_row][c_col];
				}
			}
			temp.push_back(val);
		}
		mat_out.push_back(temp);
		temp.clear();
	}
	return mat_out;
}

vector< vector<float> > MLP::matmul(float &L, vector<float> &vec_R) {
	// scaling a vector by a value
	vector< vector<float> > mat_in;
	mat_in = addDimension(vec_R);
	vector< vector<float> > mat_out;
	mat_out = matmul(L, mat_in);
	return mat_out;
}

vector< vector<float> > MLP::matmul(float &L, vector< vector<float> > &mat_R) {
	// scaling a vector by a value
	vector< vector<float> > mat_out;
	vector<float> temp;
	for (size_t c_row = 0; c_row<mat_R.size(); c_row++){
		for (size_t c_col = 0; c_col<mat_R[c_row].size(); c_col++){
			temp.push_back(L * mat_R[c_row][c_col]);
		}
		mat_out.push_back(temp);
		temp.clear();
	}
	return mat_out;
}

vector< vector<float> > MLP::matmul(vector< vector<float> > &mat_L, vector<float> &vec_R) {
	vector< vector<float> > mat_R = addDimension(vec_R);
	vector< vector<float> > mat_R_trans = transpose(mat_R);
	// cout << "mat_R: " << endl;
	// printMatrix(mat_R_trans);
	// cout << "Mat_L: " << endl;
	// printMatrix(mat_L);
	vector< vector<float> > mat_out;
	// cout << "THIS ONE?" << endl;
	mat_out = matmul(mat_L, mat_R_trans);
	return mat_out;
}

vector< vector<float> > MLP::elementwiseMul(vector< vector<float> > &mat_L, vector< vector<float> > &mat_R) {
	vector< vector<float> > mat_out;
	vector<float> temp;
	// cout << "WE ARE HERE" << endl;
	// printMatrix(mat_L);
	// printMatrix(mat_R);
	for (size_t i_row=0; i_row<mat_L.size(); i_row++) {
		temp.clear();
		for (size_t i_col=0; i_col<mat_L[i_row].size(); i_col++) {
			temp.push_back(mat_L[i_row][i_col] * mat_R[i_row][i_col]);
		}
		mat_out.push_back(temp);
	}
	return mat_out;
}

vector< vector<float> > MLP::elementwiseMul(vector<float> &vec_L, vector< vector<float> > &mat_R) {
	vector< vector<float> > mat_out;
	vector< vector<float> > mat_L = addDimension(vec_L);
	mat_out = elementwiseMul(mat_L, mat_R);
	return mat_out;
}

vector< vector<float> > MLP::elementwiseMul(vector< vector<float> > &mat_L, vector<float> &vec_R) {
	vector< vector<float> > mat_out;
	vector< vector<float> > mat_R = addDimension(vec_R);
	mat_out = elementwiseMul(mat_L, mat_R);
	return mat_out;
}

void MLP::matAdd(vector< vector<float> > &mat_L, vector< vector<float> > &mat_R, vector< vector<float> > &mat_out) {
	size_t rows = mat_L.size();
	size_t cols = mat_L[0].size();
	for (size_t i_row=0; i_row<rows; i_row++) {
		for (size_t i_col=0; i_col<cols; i_col++){
			mat_out[i_row][i_col] = mat_L[i_row][i_col] + mat_R[i_row][i_col];
		}
	}
}

vector<float> MLP::forward_1Layer(vector<float> &in_nodes, vector< vector<float> > &weights_in) {
	// run forward through a single layer
	vector<float> sample;
	sample = add_bias_node(in_nodes);
	vector< vector<float> > sample_mat = addDimension(sample);
	vector< vector<float> > in_nodes_trans;
	// cout << "in_nodes" << endl;
	// printMatrix(sample_mat);
	in_nodes_trans = transpose(sample);
	// cout << "in_nodes_trans: " << endl;
	// printMatrix(in_nodes_trans);
	vector< vector<float> > L;
	vector<float> L_vec;
	vector<float> NL;
	// cout << "Weights In: " << endl;
	// printMatrix(weights_in);
	L = matmul(weights_in, sample_mat);
	L_vec = removeDimension(L);
	NL = sigmoid(L_vec);
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
	float epsilon = 0.00000001;
	L = ( (float)actual - log(pred_NL + epsilon) ) + ( (1.0 - (float)actual) * log(1.0 - pred_NL +epsilon) );
	// cout << L << endl;
	L = max((float)-1, min((float)1, L));
	// DEBUGEXIT();
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

void MLP::initializeMatRandom(vector< vector<float> > & mat_in, int rows, int cols) {
	// layers
	float init_c = 1;
	float init_spread = 2;
	default_random_engine generator(100);
	uniform_real_distribution<float> distribution(init_c,init_spread);

	vector<float> temp;
	for (size_t i1 = 0; i1 < rows; i1++) {
		temp.clear();
		for (size_t i2 = 0; i2 < cols; i2++) {
			temp.push_back(distribution(generator));
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

	for (int it = 0; it<iters; it++) {
		cout << "Iteration: " << it << endl;
		stringstream batch_fn;
		batch_fn << Folder << "Batch_" << it << ".csv";

		openCSVFile(batch_recorder, batch_fn.str());
		
		/////////////////FORWARD//////////////////////
		dL_w2.clear();
		initializeMat(dL_w2, W2.size(), W2[0].size());
		// dL_w2 = W2;
		dL_w1.clear();
		initializeMat(dL_w1, W1.size(), W1[0].size());
		float L = 0;
		float delta_i = 0;
		vector<float> NL1;
		vector<float> NL1B;
		vector<float> NL2; // single value
		int acc = 0; // number of correct preditions
		for (size_t ib = 0; ib<batch_size; ib++) {
			// cout << "Batch Number: " << ib << endl;
			int sample_idx;
			uniform_int_distribution<int> batch_sample_distribution(0,data.size()-1);
			sample_idx = batch_sample_distribution(generator);
			vector<int> sample = data[sample_idx];
			// cout << "Sample: " << sample_idx << endl;
			// printMatrix(sample);

			/* 
			LAYER 1
			*/
			NL1.clear();
			NL1 = forward_1Layer(sample, W1);
			cout << "W1: " << endl;
			printMatrix(W1);
			cout << "NL1: " << endl;
			printMatrix(NL1);
			// DEBUG1();

			/* 
			LAYER 2
			*/
			// cout << "Layer 2" << endl;
			NL2.clear();
			cout << "W2: " << endl;
			printMatrix(W2);
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
			cout << "Output: " << out[0] << ", Actual: " << results[sample_idx] << ", Loss: " << L << endl;

			delta_i = (float) ((float)results[sample_idx] - NL2[0]);
			NL1B.clear();
			NL1B = add_bias_node(NL1);
			if (isnan(NL1B[0])) {
				cout << "Nan found" << endl;
				exit(EXIT_FAILURE);
			}
			dL_w2 = matmul(delta_i, NL1B);
			
			/////////////////BACKWARD//////////////////////
			vector<float> sampleB;
			sampleB = add_bias_node(sample);

			vector<float> dNL1_dW;
			for (auto & v: NL1) {
				dNL1_dW.push_back(v*(1-v));
			}
			float output_error;
			output_error = results[sample_idx] - NL2[0];
			
			vector< vector<float> > dW2_dy;
			vector< vector<float> > W2_withoutBias;
			W2_withoutBias = removeBiasWeights(W2);
			// cout << "W2 without Bias Unit Weights: ";
			// printMatrix(W2_withoutBias);
			dW2_dy = matmul(output_error, W2_withoutBias);
			// cout << "dW2_dy: ";
			// printMatrix(dW2_dy);
			vector< vector<float> > dW2_dy_trans = transpose(dW2_dy);
			// cout << "dNL1_dW" <<endl;
			// printMatrix(dNL1_dW);
			// cout << "dW2_dy" <<endl;
			// printMatrix(dW2_dy);
			// cout << "dW2_dy_trans" <<endl;
			// printMatrix(dW2_dy_trans);
			vector< vector<float> > dNL1_dW_dW2_dy;
			// DEBUG2();
			dNL1_dW_dW2_dy = elementwiseMul(dNL1_dW, dW2_dy_trans);
			vector< vector<float> > dNL1_dW_dW2_dy_sampleBias;
			dNL1_dW_dW2_dy_sampleBias = matmul(dNL1_dW_dW2_dy, sampleB);
			// dL_w1 += dNL1_dW_dW2_dy_sampleBias;
			// printMatrix(dNL1_dW_dW2_dy_sampleBias);
			matAdd(dL_w1, dNL1_dW_dW2_dy_sampleBias, dL_w1);
			// cout << "output_error" << endl;
			// cout << output_error << endl;
			// cout << "sampleB" << endl;
			// printMatrix(sampleB);
			// cout << "dNL1_dW_dW2_dy" << endl;
			// printMatrix(dNL1_dW_dW2_dy);
			// cout << "dNL1_dW_dW2_dy_sampleBias" << endl;
			// printMatrix(dNL1_dW_dW2_dy_sampleBias);
			// cout << "dL_w1" << endl;
			// printMatrix(dL_w1);
			// DEBUGEXIT();
			// if (ib > 100) {
			// 	DEBUGEXIT();
			// }
			
			
			// for (size_t i_d = 0; i_d<NL1.size()-1; i_d++) { // each hidden node
				// float dNL1_dW = NL1[i_d]*(1-NL1[i_d]);
				// float dW2_dy = 0;
				// for (size_t i_w=0; i_w < W2[i_d].size(); i_w++) { // a column vector
				// 	dW2_dy += W2[i_d][i_w]*(results[sample_idx] - NL2[0]);
				// }
				// for (size_t i_s = 0; i_s<sample.size(); i_s++) { // each input
				// 	// for (size_t i_w=0; i_w<dL_w1[i_d].size(); i_d++) { // multiply by samples input to get weight matrix
				// 	// 	dL_w1[i_d][i_w] += ( dNL1_dW * dW2_dy) * sampleB[i_d];
				// 	// }
				// 	dL_w1[i_d][i_s] += ( dNL1_dW * dW2_dy) * sampleB[i_s];
				// }
			// }
			// DEBUG2();
			// Write values to file
			// batch_recorder << results[sample_idx] << ", " << out[0] << endl;
		}

		///////////// WRITE results to CSV File/////////////
		closeCSVFile(batch_recorder);
		overall_recorder << it << ", " << (float)acc/(float)batch_size << ", " << L << endl;

		/////////////////UPDATE STEP//////////////////////
		// cout << "dL_w1" << endl;
		// printMatrix(dL_w1);
		// cout << "dL_w2" << endl;
		// printMatrix(dL_w2);
		// DEBUGEXIT();
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
		// cout << "W1" << endl;
		// printMatrix(W1);
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
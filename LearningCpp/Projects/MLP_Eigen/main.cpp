#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <math.h>
#include <sstream>


using namespace std;
using namespace Eigen;


#define DEBUG1() cout<<"Debug 1"<<endl;
#define DEBUG2() cout<<"Debug 2"<<endl;
#define DEBUG3() cout<<"Debug 3"<<endl;
#define DEBUGEXIT() exit(EXIT_FAILURE);

/*
GENERAL NOTES FOR WORKING WITH EIGEN:

1) specify the dimensions of all matrices up front
2) scalar multiplication -- use the matrix * scalar, don't use array
3) write stuff to log files to make it easier to track issues
4) NaNs can cause problems and be hard to find
*/




float sigmoid(float &in_val) {
	float out_val;
	float exp_val;
	exp_val = exp((double) -in_val);
	out_val = 1.0/(1.0 + exp_val);
	return out_val;
}

// MatrixXf sigmoid(MatrixXf &in_mat) {
// This needs to have a custom function that does the sigmoid function
// 	MatrixXf out_mat;
// 	MatrixXf exp_mat;
// 	exp_mat = in_mat.array().exp();
// 	out_mat = 1.0/(1.0 + in_mat.array());
// 	// exp_val = exp((double) -in_val);
// 	// out_val = 1.0/(1.0 + exp_val);
// 	return out_mat;
// }

MatrixXf sigmoid(MatrixXf &in_mat) {
	MatrixXf out_mat;
	MatrixXf sig_mat = in_mat;
	for (int r = 0; r<in_mat.rows(); r++) {
		for (int c = 0; c<in_mat.cols(); c++) {
			sig_mat(r,c) = sigmoid(in_mat(r,c));
		}
	}
	return sig_mat;
}

void printSize(MatrixXf &in_mat) {
	cout << "R: " << in_mat.rows();
	cout << ", L: " << in_mat.cols();
	cout << endl;
}

void writeDataset(string const &fn, MatrixXf const &data, MatrixXf &output) {
	// write dataset to file
	std::ofstream dataset;
	dataset.open(fn);
	for (size_t r=0; r<data.rows(); r++) {
		for (size_t c=0; c<data.cols(); c++) {
			dataset << data(r,c) << ", ";
		}
		dataset << output(r) << std::endl;
	}
	dataset.close();
}

void createAndData(MatrixXf const &data, MatrixXf &output) {
	string fn = "And.csv";
	for (size_t i=0; i<data.rows(); i++) {
		if (data.row(i).sum() == data.row(i).size()) {
			output(i) = 1;
			
		} else {
			output(i) = 0;
		}
	}
}

void createXORData(MatrixXf const &data, MatrixXf &output) {
	string fn = "XOR.csv";
	for (size_t i=0; i<data.rows(); i++) {
		if (data(i,0) == data(i,1)) {
			output(i) = 1;
			
		} else {
			output(i) = 0;
		}
	}
	writeDataset(fn, data, output);
}

void loadDataset(string const &fn, MatrixXf const &data, MatrixXf &output) {
	// load dataset from file
	std::ifstream dataset;
	dataset.open(fn);
	// read each line into a string stream
	// parse line
	// set values of arrays
	dataset.close();
}


int main() {
	int data_points = 1000;
	int in_dims = 2;
	int out_dims = 1;

	MatrixXf data(data_points, in_dims);
	MatrixXf output(data_points, out_dims);

	srand(0);
	for (int i=0; i<data_points; i++) {
		data(i,0) = rand() % 2; // values either 0 and 1
		data(i,1) = rand() % 2;
	}

	string fn = "XOR.csv";
	// // And Data set
	// createAndData(data, output);

	// XOR Dataset
	// createXORData(data,output);
	

	int hidden_nodes_ct = 5;
	int input_dims_ct = in_dims;
	int output_dims_ct = out_dims;
	int bias_in = 1;
	MatrixXf bias_vec(1,1);
	bias_vec << bias_in;

	float learning_rate = 1.5;
	int batch_size = 10.0;
	int iters = 1000;


	MatrixXf W1 = MatrixXf::Random(hidden_nodes_ct, input_dims_ct+1);
	MatrixXf W2 = MatrixXf::Random(output_dims_ct, hidden_nodes_ct+1);
	MatrixXf dL_w2 = W2;
	MatrixXf dL_w1 = W1;


	cout << "Batch Size: " << batch_size << endl;
	cout << "Iterations: " << iters << endl;
	cout << "W1" << endl;
	cout << W1 << endl;
	cout << "W2" << endl;
	cout << W2 << endl;

	stringstream overall_fn;
	overall_fn << "Overall" << ".csv";
	ofstream overall_recorder;
	overall_recorder.open(overall_fn.str(), ofstream::out);
	float L = 0;
	int actual_result = 0;
	float acc = 0;
	uniform_int_distribution<int> batch_sample_distribution(0,data.rows()-1);
	default_random_engine generator(100);


	// initialize all matrices
	MatrixXf L1(hidden_nodes_ct,1);
	MatrixXf NL1 = L1;
	MatrixXf NL1B(NL1.rows()+1,NL1.cols());
	MatrixXf L2(output_dims_ct,1);
	MatrixXf NL2 = L1;
	for (int it = 0; it<iters; it++) {
		cout << "Iteration: " << it << "\t";
		acc = 0;
		// zero out step arrays
		dL_w2.setZero();
		dL_w1.setZero();

		// L = 0;

		for (size_t ib = 0; ib<batch_size; ib++) {
			// pick sample
			int sample_idx;
			sample_idx = batch_sample_distribution(generator); 
			// sample_idx = ib;
			actual_result = output(sample_idx);
			MatrixXf sample(2,1);
			sample << data.row(sample_idx).transpose();
			// cout << "Sample " << sample_idx << "\t" ;
			// cout << sample.transpose() << "\t";
			// cout << "Result " << actual_result << endl;
			// cout << bias_vec << endl;
			MatrixXf sampleb(sample.rows()+bias_vec.rows(),1);
			// DEBUG2();
			sampleb << sample, bias_vec;
			// sampleb
			// cout << "Bias Sample: " << endl;
			// cout << sampleb << endl;
			// MatrixXf L1, NL1;
			L1 = W1*sampleb;
			NL1 = sigmoid(L1);

			// MatrixXf NL1B(NL1.rows()+1,NL1.cols());
			NL1B << NL1, 1.0;

			// cout << "L1" << endl;
			// cout << L1 << endl;
			// cout << "NL1" << endl;
			// cout << NL1 << endl;
			// cout << "NL1B" << endl;
			// cout << NL1B << endl;

			// MatrixXf L2, NL2;
			L2 = W2*NL1B;
			NL2 = sigmoid(L2);

			// cout << "L2" << endl;
			// cout << L2 << endl;
			// cout << "NL2" << endl;
			// cout << NL2 << endl;

			// classification step -- do this with built in matrix func?
			float pred_class;
			if (NL2(0,0)> 0.5) {
				pred_class = 1;
			} else {
				pred_class = 0;
			}
			// cout << "Predicted Class: " << pred_class << "\t";
			// cout << "Actual Result: " << actual_result << endl;
			if (pred_class == actual_result) {
				acc++;
			}

			// cout << "Class: " << output << endl;

			MatrixXf neg_NL2, neg_NL1;
			neg_NL2 = 1.0-NL2.array();
			neg_NL1 = 1.0-NL1.array();
			// cout << "Neg: " << neg << endl;
			// cout << "log NL2: " << NL2.array().log() << endl;
			// printSize(neg);
			MatrixXf Loss;
			// cout << actual_result << ", ";
			// cout << NL2(0,0) << ", ";
			// cout << actual_result * NL2.array().log10() << ", ";
			// cout << (1 - actual_result) * neg.array().log10() << endl;
			Loss = actual_result * NL2.array().log10() + (1 - actual_result) * neg_NL2.array().log10();
			if (isnan(Loss(0,0))) {
				cout << "NAN" << endl;
			}
			// cout << "Loss: " << Loss << endl;
			L -= Loss(0,0);
			MatrixXf delta_i;
			delta_i = actual_result - NL2.array();
			// cout << "Delta_I: " << delta_i.array() << endl;
			// printSize(delta_i);
			MatrixXf NL1B_T;
			NL1B_T = NL1B;
			// NL1B_T.transpose();
			// cout << "NL1B': " << NL1B_T << endl;

			dL_w2 = dL_w2 + delta_i*NL1B.transpose();
			// cout << "dL_w2: " << dL_w2 << endl;
			// printSize(NL1);
			// printSize(neg);
			MatrixXf dNL1_dW = NL1.array()*neg_NL1.array();
			MatrixXf output_error = actual_result - NL2.array();
			// cout << "Output Error: " << output_error << endl;
			// printSize(W2);
			// cout << "Weights without Bias: " << W2.block(0,0,W2.rows(), W2.cols()-1)  << endl;

			MatrixXf dW2_dy = W2.block(0,0,W2.rows(), W2.cols()-1).array() * output_error(0,0);
			
			// printSize(dNL1_dW);
			// printSize(dW2_dy);
			MatrixXf dNL1_dW_dW2_dy = dNL1_dW.array() * dW2_dy.transpose().array();

			// printSize(dNL1_dW_dW2_dy);
			// printSize(sampleb);
			// printSize(dL_w1);
			dL_w1 = dL_w1 + dNL1_dW_dW2_dy * sampleb.transpose();

			// overall_recorder << sample(0) << ", " << sample(1) << ", ";
			// overall_recorder << pred_class << ", " << actual_result << ", ";
			// overall_recorder << Loss << ", " << L << ", ";
			// overall_recorder << NL2(0,0) << ", ";
			// overall_recorder << (float)acc/(float)batch_size * 100.0 << endl;

			// DEBUGEXIT();

		}

	// cout << "W1" << endl;
	// cout << W1 << endl;
	// cout << "W2" << endl;
	// cout << W2 << endl;
		W2 = W2 + learning_rate * dL_w2/batch_size;
		W1 = W1 + learning_rate * dL_w1/batch_size;

	// cout << "W1" << endl;
	// cout << W1 << endl;
	// cout << "W2" << endl;
	// cout << W2 << endl;

		// cout << "Accuracy: " << acc << endl;
		cout << "Accuracy: " << (float)acc/(float)batch_size * 100.0 << "%" << "\t";
		cout << "Loss: " << L << endl;
		overall_recorder << it << ", ";
		overall_recorder << (float)acc/(float)batch_size * 100.0 << ", ";
		overall_recorder << L << endl;
		
		// DEBUGEXIT();

	}

	DEBUG1();
	cout << "W1" << endl;
	cout << W1 << endl;
	cout << "W2" << endl;
	cout << W2 << endl;
	overall_recorder.close();





	
}
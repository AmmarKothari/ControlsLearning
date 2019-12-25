#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#define vec_2d_float vector< vec_float>
#define vec_float vector<float>

void write_matrix_to_csv(vec_2d_float xy_data, string output_file_name){
	ofstream output_file;
	output_file.open(output_file_name);
	for (auto xy = xy_data.begin(); xy != xy_data.end(); xy++){
		for (auto val = xy->begin(); val != xy->end()-1; val++){
			output_file << *val << ", ";
		};
		output_file << xy->back() << "\n";
	};
	output_file.close();
};

void write_vector_to_csv(vec_float in_data, string output_file_name){
	ofstream output_file;
	output_file.open(output_file_name);
	for (auto val = in_data.begin(); val != in_data.end(); val++){
		output_file << *val << "\n";
	};
	output_file.close();
}
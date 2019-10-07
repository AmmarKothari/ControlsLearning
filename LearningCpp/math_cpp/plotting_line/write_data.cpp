#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

const float MAX_TIME_VAL = 5;
const float TIME_STEP = 0.01;
const string INPUT_FILE = "input_file.csv";

#define vec_float vector<float>
#define vec_2d_float vector< vec_float>

vec_2d_float get_sin_data(){
	vec_2d_float xy_data;
	vec_float xy;
	// TODO: change this so we can just overwrite instead of create and destory
	float val;
	while(val < MAX_TIME_VAL){
		xy.push_back(val);
		xy.push_back(sin(val));
		xy_data.push_back(xy);
		xy.clear();
		val += TIME_STEP;
	};
	return xy_data;
};


void write_matrix_to_csv(vec_2d_float xy_data){
	std::ofstream output_file;
	output_file.open(INPUT_FILE);

	for (auto xy = xy_data.begin(); xy != xy_data.end(); xy++){
		for (auto val = xy->begin(); val != xy->end()-1; val++){
			output_file << *val << ", ";
		};
		output_file << xy->back() << "\n";
	};
	output_file.close();
};


int main () {
	vec_2d_float xy_data = get_sin_data();
	write_matrix_to_csv(xy_data);
	
	return 0;
}
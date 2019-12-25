#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

#include "../utils/write_to_csv.cpp"
#include "../utils/gen_sin_data.cpp"

using namespace std;

#define vec_float vector<float>
#define vec_2d_float vector< vec_float>

const float MAX_TIME_VAL = 5;
const float TIME_STEP = 0.01;
const string OUTPUT_FILE = "output_file.csv";


int main () {
	vec_2d_float xy_data = get_sin_data(MAX_TIME_VAL, TIME_STEP);
	write_matrix_to_csv(xy_data, OUTPUT_FILE);
	
	return 0;
}
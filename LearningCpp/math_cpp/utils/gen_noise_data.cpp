#include <random>

// #include "./write_to_csv.cpp"
#include "./vector_ops.cpp"

using namespace std;

#define vec_float vector<float>
#define vec_2d_float vector< vec_float>

vector<float> gen_1d_random_distribution(float mean_value, float stdev, int points) {
	// Generate a gaussian distribution around mean_value with stdev
	default_random_engine generator;
	normal_distribution<float> distribution(mean_value, stdev);
	vector<float> data;
	for (auto i=0; i<points; i++) {
		data.push_back(distribution(generator));
	}
	return data;
}

vector<float> gen_sin_1d_data(float max_time, float time_step){
	vec_float xy;
	float val = 0.0;
	while(val < max_time){
		xy.push_back(sin(val));
		val += time_step;
	};
}

vector<float> get_noisey_1d_sin_data(float true_value, float max_time, float time_step){
	// Generate 1D noisy data of a sin wave
	vec_float noisey_sin_data;
	vec_float sin_data = gen_sin_1d_data(max_time, time_step);
	vec_float noisey_1d_data = gen_1d_random_distribution(0, 0.1, sin_data.size());
	vec_2d_float zipped_data = zip(sin_data, noisey_1d_data);
	// float val = 0.0;
	// while(val < max_time){
	// 	noisey_sin_data.push_back(val);
	// 	xy.push_back(sin(val) + true_value);
	// 	xy_data.push_back(xy);
	// 	xy.clear();
	// 	val += time_step;
	// };
	// return xy_data;
};

// int main() {
// 	vector<float> out_data = gen_1d_random_distribution(1, 1, 100);
// 	// write_vector_to_csv(out_data, "test_output.csv");
// 	return 0;
// }



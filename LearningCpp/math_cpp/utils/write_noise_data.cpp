#include "gen_noise_data.cpp"
#include "write_to_csv.cpp"

void write_one_d_data_to_csv(){
	vector<float> oned_rand = gen_1d_random_distribution(1.0, 1.0, 1000);
	write_vector_to_csv(oned_rand, "one_d_noisy.csv");
}

void write_sin_data_to_csv(){
	vec_2d_float sin_noisey = get_noisey_1d_sin_data(1, 10, 0.01);
	write_matrix_to_csv(sin_noisey, "sin_noisy_data.csv");
}

int main() {
	write_one_d_data_to_csv();
	write_sin_data_to_csv();
	return 0;
}
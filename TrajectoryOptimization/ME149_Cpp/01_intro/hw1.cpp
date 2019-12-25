#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include <random>
#include <string>

#include "../../../LearningCpp/math_cpp/utils/write_to_csv.cpp"

float CIRCLE_RADIUS = 1.0;

vec_2d_float get_control_points(int num_control_points){
    vec_2d_float control_points;
    std::vector<float> control_point;
    control_point.assign(2, 0.0);
    for (int i =0; i<num_control_points; i++) {
        float theta = 2*M_PI * float(i) / num_control_points;
        float x = std::sin(theta) * CIRCLE_RADIUS;
        float y = std::cos(theta) * CIRCLE_RADIUS;
        control_point[0] = x;
        control_point[1] = y;
        control_points.push_back(control_point);
    }
    return control_points;
}

vec_2d_float get_sample_points(int num_sample_points, float alpha, vec_2d_float control_points){
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,control_points.size()-1);

    std::vector<float> previous_point = control_points.front();
    vec_2d_float sample_points;
    int rand_ind;
    float x_prev, y_prev, x_new, y_new;
    std::vector<float> control_point;
    std::vector<float> new_point = {0.0, 0.0};
    for (int p = 0; p < num_sample_points; p++){
        rand_ind = distribution(generator);
        x_prev = previous_point.at(0);
        y_prev = previous_point.at(1);
        control_point = control_points.at(rand_ind);
        x_new = alpha * x_prev + (1.0-alpha) * control_point[0];
        y_new = alpha * y_prev + (1.0-alpha) * control_point[1];
        new_point[0] = x_new;
        new_point[1] = y_new;
        sample_points.push_back(new_point);
        previous_point = sample_points.back();
        std::cout << previous_point[0] << ", " << previous_point[1] << std::endl;
    }
    return sample_points;
}

void fractal_drawing(int num_control_points, float alpha, int num_sample_points, string filename_prefix){
    vec_2d_float control_points;
    control_points = get_control_points(num_control_points);
    string control_points_filename = filename_prefix + "_control_points.csv";
    write_matrix_to_csv(control_points, control_points_filename);
    vec_2d_float sample_points = get_sample_points(num_sample_points, alpha, control_points);
    string sample_points_filename = filename_prefix + "_sample_fractal_points.csv";
    write_matrix_to_csv(sample_points, sample_points_filename);
}


int main(){
    fractal_drawing(3, 0.5, 500, "part_1");
    fractal_drawing(4, 0.33333333, 500, "part_2");
}
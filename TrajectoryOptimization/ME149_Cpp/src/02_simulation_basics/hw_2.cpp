#include <random>

#include "../modelSystem/simple_pendulum/simple_pendulum.cpp"
#include "../modelSystem/lorenz/lorenz.cpp"
#include "../simulation/euler_method_sim.cpp"
#include "vector_ops.cpp"
#include "write_to_csv.cpp"

#define STARTING_MIN_ANGLE -3.0
#define STARTING_MAX_ANGLE 3.0
#define STARTING_MIN_ANGLE_RATE -1.0
#define STARTING_MAX_ANGLE_RATE 1.0

void prob_03(){
    float t_span[2] = {0.0, 20.0};
    float t_max_step = 0.01;

    srand(1000);
    std::default_random_engine generator;
    std::uniform_real_distribution<float> angle_random_distribution(STARTING_MIN_ANGLE, STARTING_MAX_ANGLE);
    std::uniform_real_distribution<float> angle_rate_random_distribution(STARTING_MIN_ANGLE_RATE, STARTING_MAX_ANGLE_RATE);
    std::vector<float> state_0 = {angle_random_distribution(generator), angle_rate_random_distribution(generator)};
    
    state_type z_grid;
    std::vector<float> t_grid;
    
    euler_method_sim(simple_pendulum_forward_dynamics, t_span, state_0, t_max_step, z_grid, t_grid);
    state_vector_type z_dot_grid;
    z_dot_grid = simple_pendulum_forward_dynamics_vectorized(z_grid, t_grid);

    // Assemble the full state vector
    state_vector_type all_states; // q, dq, ddq
    state_type single_state;
    state_type single_state_dot;
    for (int i=0; i<z_grid.size(); i++){
        single_state = z_grid[i];
        single_state_dot = z_dot_grid[i];
        single_state.push_back(single_state_dot.back());
        all_states.push_back(single_state);
    }

    // Write state vector and time to file
    std::vector<std::vector<float>> all_data;
    all_data = zip(t_grid, all_states);
    write_matrix_to_csv(all_data, "problem_03.csv");

}

#define X_MIN 5.0
#define X_MAX 35.0
#define Y_MIN -30.0
#define Y_MAX 5.0
#define Z_MIN -5.0
#define Z_MAX 35.0
#define sigma 10.0
#define rho 29.0
#define beta 8.0/3.0

void prob_04(){
    float t_span[2] = {0.0, 10.0};
    float t_max_step = 0.005;

    srand(1000);
    std::default_random_engine generator;
    std::uniform_real_distribution<float> x(X_MIN, X_MAX);
    std::uniform_real_distribution<float> y(Y_MIN, Y_MAX);
    std::uniform_real_distribution<float> z(Z_MIN, Z_MAX);
    state_type state_0 = {x(generator), y(generator), z(generator)};
    
    state_vector_type z_grid;
    std::vector<float> t_grid;
    
    auto lorenz_forward_dynamics_partial = [](state_type state, float t) {return lorenz_forward_dynamics(state, t, sigma, rho, beta);};
    euler_method_sim(lorenz_forward_dynamics_partial, t_span, state_0, t_max_step, z_grid, t_grid);
    
    state_vector_type z_dot_grid;
    z_dot_grid = lorenz_forward_dynamics_vectorized(z_grid, t_grid);

    // Assemble the full state vector
    state_vector_type all_states; // q, dq, ddq
    state_type single_state;
    state_type single_state_dot;
    for (int i=0; i<z_grid.size(); i++){
        single_state = z_grid[i];
        single_state_dot = z_dot_grid[i];
        single_state.push_back(single_state_dot.back());
        all_states.push_back(single_state);
    }

    // Write state vector and time to file
    std::vector<std::vector<float>> all_data;
    all_data = zip(t_grid, z_grid);
    write_matrix_to_csv(all_data, "problem_04.csv");

}

int main(){
    prob_03();
    prob_04();
}
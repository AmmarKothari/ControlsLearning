#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>

#include "lorenz.h"
#include "../../simulation/eulers_method/euler_method_sim.h"

TEST_CASE("Lorenz init", "[lorenz_init]"){
    Lorenz *lorenz_model = new Lorenz;
    Lorenz *lorenz_model2 = new Lorenz(1.0, 1.0, 0.0);
}

TEST_CASE("Lorenze forward dynamics", "[lorenz_forward]"){
    Lorenz *lorenz_model = new Lorenz;
    state_type state_0 = {1.0, 2.0, 3.0};
    state_type d_state;
    d_state = lorenz_model->forward_dynamics(state_0, 0.1);
    std::cout << d_state[0] << ", " << d_state[1] << ", " << d_state[2] << std::endl;
}

TEST_CASE("Lorenz euler function", "[lorenz_euler_func]"){
    float sigma = 10.0;
    float rho = 29;
    float beta = 8.0/3.0;

    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0, 1.0, 2.0};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    
    // Lambdas function
    auto lorenz_forward_dynamics_partial = [sigma, rho, beta](state_type state, float t) {return lorenz_forward_dynamics(state, t, sigma, rho, beta);};
    euler_method_sim(lorenz_forward_dynamics_partial, t_span, x0, t_max_step, z_grid, t_grid);
    for (auto& step: z_grid){
        for (auto& s: step){
            std::cout << s << ", ";
        }
        std::cout << std::endl;
    }
}

TEST_CASE("Lorenz euler object", "[lorenz_euler_object]"){
    float sigma = 10.0;
    float rho = 29;
    float beta = 8.0/3.0;
    Lorenz *lorenz_model = new Lorenz(sigma, rho, beta);

    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0, 1.0, 2.0};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    
    euler_method_sim(lorenz_model, t_span, x0, t_max_step, z_grid, t_grid);
    for (auto& step: z_grid){
        for (auto& s: step){
            std::cout << s << ", ";
        }
        std::cout << std::endl;
    }
}
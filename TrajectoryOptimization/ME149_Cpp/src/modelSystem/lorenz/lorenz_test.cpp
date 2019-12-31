#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>

#include "lorenz.h"
#include "../../simulation/eulers_method/euler_method_sim.h"



TEST_CASE("Simple pendulum euler method", "[simple_pendulum_euler_method]"){
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
    // bind
    euler_method_sim(lorenz_forward_dynamics_partial, t_span, x0, t_max_step, z_grid, t_grid);

}
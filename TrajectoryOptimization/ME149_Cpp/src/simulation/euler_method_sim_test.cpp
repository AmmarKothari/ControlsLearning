#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>
#include "euler_method_sim.cpp"
#include <vector>

std::vector<float> test_dynamics(std::vector<float> &x, float t){
    std::vector<float> dx;
    dx.push_back(10.0);
    return dx;
}

TEST_CASE("sim test", "[sim_test]"){
    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    euler_method_sim(test_dynamics, t_span, x0, t_max_step, z_grid, t_grid);
    REQUIRE(t_grid[0] == Approx(0.0));
    state_type first_step = z_grid[0];
    REQUIRE(first_step[0] == Approx(0.0));
    REQUIRE(z_grid.size() == 11);

    state_type last_step = z_grid.back();
    REQUIRE(last_step[0] == Approx(10.0));

    
}
    
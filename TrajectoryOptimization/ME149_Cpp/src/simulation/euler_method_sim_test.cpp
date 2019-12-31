#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>
#include "euler_method_sim.h"
#include <vector>

#include "../modelSystem/dynamics_model.h"

std::vector<float> test_dynamics(state_type &x, float t){
    std::vector<float> dx;
    dx.push_back(10.0);
   return dx;
}

std::vector<float> test_dynamics_unused_var(state_type &x, float t, float unused){
    std::vector<float> dx;
    dx = test_dynamics(x, t);
   return dx;
}

class TestSystem : public DynamicsModel{
    public:
        TestSystem(){
            std::cout << "Created Test system object" << std::endl;
        }

        state_type forward_dynamics(state_type &x, float t){
            state_type dx;
            dx.push_back(10.0);
            std::cout << "adding value" << std::endl;
            return dx;
        }
};

TEST_CASE("function test", "[function_test]"){
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

TEST_CASE("lambda test", "[lambda_test]"){
    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;

    float unused_var = 1.0;

    auto test_dynamics_lambda = [&, unused_var](state_type state, float t){return test_dynamics_unused_var(state, t, unused_var);};

    euler_method_sim(test_dynamics_lambda, t_span, x0, t_max_step, z_grid, t_grid);
    REQUIRE(t_grid[0] == Approx(0.0));
    state_type first_step = z_grid[0];
    REQUIRE(first_step[0] == Approx(0.0));
    REQUIRE(z_grid.size() == 11);

    state_type last_step = z_grid.back();
    REQUIRE(last_step[0] == Approx(10.0));
}

TEST_CASE("Object test", "[object_test]"){
    DynamicsModel* test_system = new TestSystem();

    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;

    test_system->forward_dynamics(x0, t_span[0]);
    euler_method_sim(test_system, t_span, x0, t_max_step, z_grid, t_grid);
    REQUIRE(t_grid[0] == Approx(0.0));
    state_type first_step = z_grid[0];
    REQUIRE(first_step[0] == Approx(0.0));
    REQUIRE(z_grid.size() == 11);

    // state_type last_step = z_grid.back();
    // REQUIRE(last_step[0] == Approx(10.0));
}
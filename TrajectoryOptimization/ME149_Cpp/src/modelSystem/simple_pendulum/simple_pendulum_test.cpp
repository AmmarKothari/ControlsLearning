#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <functional>

#include "simple_pendulum.cpp"
#include "../../simulation/euler_method_sim.cpp"

TEST_CASE("Simple Pendulum Initialization", "[simple_pendulum_init]"){
    SimplePendulum pendulum_1;
    REQUIRE(pendulum_1.length == float(1.0));

    SimplePendulum pendulum_2(0.5, 0.6, 0.7);
    REQUIRE(pendulum_2.length == float(0.5));
    REQUIRE(pendulum_2.mass == float(0.6));
    REQUIRE(pendulum_2.damping == float(0.7));
};

TEST_CASE("Simple pendulum dynamics", "[simple_pendulum_dynamics]"){
    SimplePendulum pendulum;
    state_type x0 = {M_PI_2, 0.0};
    state_type xd0 = {0.0, 0.0};
    state_type xd_next = {0.0, 0.0};
    xd_next = pendulum.forward_dynamics(x0, 0.1);
    REQUIRE(xd_next[0] == float(0.0));
    REQUIRE(xd_next[1] == -std::sin(x0[0]));

};

TEST_CASE("Simple pendulum euler method", "[simple_pendulum_euler_method]"){
    SimplePendulum pendulum;

    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0, M_PI_2};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    typedef state_type(SimplePendulum::*forward_dynamics_type)(state_type &, float);
    // auto dyn_func = std::bind(&SimplePendulum::forward_dynamics, &pendulum, std::placeholders::_1, std::placeholders::_2);
    
    euler_method_sim(dyn_func, t_span, x0, t_max_step, z_grid, t_grid);

}
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>
#include <functional>

#include "simple_pendulum.h"
#include "../../simulation/eulers_method/euler_method_sim.h"

TEST_CASE("Initialization", "[simple_pendulum_init]"){
    SimplePendulum pendulum_1;
    REQUIRE(pendulum_1.length == float(1.0));

    SimplePendulum pendulum_2(0.5, 0.6, 0.7);
    REQUIRE(pendulum_2.length == float(0.5));
    REQUIRE(pendulum_2.mass == float(0.6));
    REQUIRE(pendulum_2.damping == float(0.7));
};

TEST_CASE("Dynamics", "[simple_pendulum_dynamics]"){
    SimplePendulum pendulum;
    state_type x0 = {M_PI_2, 0.0};
    state_type xd_next = {0.0, 0.0};
    xd_next = pendulum.forward_dynamics(x0, 0.1);
    REQUIRE(xd_next[0] == float(0.0));
    REQUIRE(xd_next[1] == -std::sin(x0[0]));
};

// TEST_CASE ("Forward vectorized", "[simple_pendulum_forward_vectorized]"){
//     state_type state_0 = {M_PI_2, 0.0};
//     state_vector_type state_vector;
//     state_vector.assign(2, state_0);
//     std::vector<float> t_vector;
//     t_vector.assign(2, 1.0);
//     state_vector_type state_dot_vector;
//     state_dot_vector = simple_pendulum_forward_dynamics_vectorized(state_vector, t_vector);
//     REQUIRE_THAT (state_dot_vector[0], Catch::Matchers::Equals(state_dot_vector[1]));

// };

TEST_CASE ("Forward vectorized", "[simple_pendulum_forward_vectorized]"){
    SimplePendulum pendulum_1;
    state_type state_0 = {M_PI_2, 0.0};
    state_vector_type state_vector;
    state_vector.assign(2, state_0);
    std::vector<float> t_vector;
    t_vector.assign(2, 1.0);
    state_vector_type state_dot_vector;
    state_dot_vector = pendulum_1.forward_dynamics_vectorized(state_vector, t_vector);
    REQUIRE_THAT (state_dot_vector[0], Catch::Matchers::Equals(state_dot_vector[1]));

};

TEST_CASE("Euler method", "[simple_pendulum_euler_method]"){
    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0, M_PI_2};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    
    euler_method_sim(simple_pendulum_forward_dynamics, t_span, x0, t_max_step, z_grid, t_grid);
}

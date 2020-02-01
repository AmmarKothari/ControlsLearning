#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>
#include <functional>

#include "double_pendulum.h"
#include "../../simulation/eulers_method/euler_method_sim.h"

TEST_CASE("Initialization", "[double_pendulum_init]"){
    DoublePendulum pendulum_1;
    REQUIRE(pendulum_1.l1.length == float(1.0));

    Link l1 = {3.0, 2.0, 1.0};
    Link l2 = {6.0, 5.0, 4.0};
    DoublePendulum pendulum_2(l1, l2, -9.8);
    REQUIRE(pendulum_2.l1.length == float(3.0));
    REQUIRE(pendulum_2.l2.length == float(6.0));
    REQUIRE(pendulum_2.l1.mass == float(2.0));
    REQUIRE(pendulum_2.l2.mass == float(5.0));
    REQUIRE(pendulum_2.l1.damping == float(1.0));
    REQUIRE(pendulum_2.l2.damping == float(4.0));
};

TEST_CASE("Dynamics", "[double_pendulum_dynamics]"){
    DoublePendulum pendulum;
    state_type x0 = {0.0, 0.0, 0.0, 0.0};
    u_type u0 = {0.0, 0.0};
    state_type xd_next = {0.0, 0.0, 0.0, 0.0};
    xd_next = pendulum.forward_dynamics(x0, u0, 0.1);
    REQUIRE( xd_next[0] == Approx(0.0));
    REQUIRE( xd_next[1] == Approx(0.0));
};

TEST_CASE ("Forward vectorized", "[double_pendulum_forward_vectorized]"){
    DoublePendulum pendulum_1;
    state_type state_0 = {M_PI_2, 0.0, 0.0, 0.0};
    u_type u_0 = {0.0, 0.0};
    state_vector_type state_vector;
    u_vector_type u_vector;
    state_vector.assign(10, state_0);
    u_vector.assign(10, u_0);
    std::vector<float> t_vector;
    t_vector.assign(10, 1.0);
    state_vector_type state_dot_vector;
    state_dot_vector = pendulum_1.forward_dynamics_vectorized(state_vector, u_vector, t_vector);
    REQUIRE_THAT (state_dot_vector[0], Catch::Matchers::Equals(state_dot_vector[1]));

};

TEST_CASE("Euler method Object", "[double_pendulum_object_euler_method]"){
    DoublePendulum *double_pendulum = new DoublePendulum;
    float t_span[2] = {0.0, 1.0};
    float t_max_step = 0.1;
    std::vector<float> x0 = {0.0, M_PI_2};
    
    std::vector<state_type> z_grid;
    std::vector<float> t_grid;
    
    euler_method_sim(double_pendulum, t_span, x0, t_max_step, z_grid, t_grid);
}

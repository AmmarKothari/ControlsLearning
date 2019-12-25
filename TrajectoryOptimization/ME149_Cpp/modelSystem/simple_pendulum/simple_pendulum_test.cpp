#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "simple_pendulum.cpp"

TEST_CASE("Simple Pendulum Initialization", "[simple_pendulum_init]"){
    SimplePendulum pendulum_1;
    REQUIRE(pendulum_1.length == float(1.0));

    SimplePendulum pendulum_2(0.5, 0.6, 0.7);
    REQUIRE(pendulum_2.length == float(0.5));
    REQUIRE(pendulum_2.mass == float(0.6));
    REQUIRE(pendulum_2.damping == float(0.7));
};
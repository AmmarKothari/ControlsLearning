#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include <catch2/catch.hpp>
#include "utils.h"

TEST_CASE("float linspace test", "[float_linspace_test]"){
    std::vector<float> test_output;
    test_output = linspace(float(1.0), float(5.0), int(100));
    REQUIRE(test_output.front() == Approx(1.0));
    REQUIRE(test_output.back() == Approx(5.0));
    REQUIRE(test_output.size() == 100);
};

// TEST_CASE("int linspace test", "[float_linspace_test]"){
//     std::vector<float> test_output;
//     test_output = linspace(int(1), int(5), int(100));
//     REQUIRE(test_output.front() == Approx(1.0));
//     REQUIRE(test_output.back() == Approx(5.0));
//     REQUIRE(test_output.size() == 100);

// };

// TEST_CASE("range test", "[range_test]"){
//     std::vector<float> test_output;
//     test_output = arange(1.0, 5.0, 0.3);
//     REQUIRE(test_output.front() == Approx(1.0));
//     REQUIRE(test_output.front() != Approx(5.0));
//     REQUIRE(test_output[1] - test_output[0] == Approx(0.3));  
// };
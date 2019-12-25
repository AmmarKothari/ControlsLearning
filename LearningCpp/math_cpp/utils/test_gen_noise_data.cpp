#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include "gen_noise_data.cpp"

TEST_CASE( "1D random distribution", "[1d_rand]" ){
	vector<float> oned_rand = gen_1d_random_distribution(1.0, 1.0, 100);
	REQUIRE( oned_rand[0] >-100.0);
	REQUIRE( oned_rand.size() == 100);
};


int main( int argc, char* argv[] ) {
  int result = Catch::Session().run( argc, argv );

  return result;
}
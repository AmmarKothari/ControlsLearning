#include "simple_pendulum.h"
#include <cmath>

SimplePendulum::SimplePendulum(float length_in, float mass_in, float damping_in){
    this->length = length_in;
    this-> mass = mass_in;
    this-> damping = damping_in;
};

state_type SimplePendulum::forward_dynamics(state_type &x ,state_type &dx ,float t ){
    dx[0] = x[1];
    dx[1] = -std::sin(x[0]);

}
#include "simple_pendulum.h"

SimplePendulum::SimplePendulum(float length_in, float mass_in, float damping_in){
    this->length = length_in;
    this-> mass = mass_in;
    this-> damping = damping_in;
};

state_type SimplePendulum::forward_dynamics(state_type &x, float t){
    // Should explicitly show the effect of gravity here!
    state_type dx_next = {0.0, 0.0};
    dx_next[0] = x[1];
    dx_next[1] = -std::sin(x[0]);
    return dx_next;
};
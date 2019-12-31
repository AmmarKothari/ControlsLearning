#include "simple_pendulum.h"

SimplePendulum::SimplePendulum(float length_in, float mass_in, float damping_in){
    this->length = length_in;
    this-> mass = mass_in;
    this-> damping = damping_in;
};

state_type SimplePendulum::forward_dynamics(state_type &x, float t){
    state_type dx_next;
    dx_next = simple_pendulum_forward_dynamics(x, t);
    return dx_next;
};

state_type simple_pendulum_forward_dynamics(state_type &x, float t){
    // Should explicitly show the effect of gravity here!
    state_type dx_next = {0.0, 0.0};
    dx_next[0] = x[1];
    dx_next[1] = -std::sin(x[0]);
    return dx_next;
};

state_vector_type simple_pendulum_forward_dynamics_vectorized(state_vector_type &state_vector, time_vector_type t_vector){
    state_vector_type state_dot_vector;
    state_type d_state_next;
    for (size_t i=0; i < state_vector.size(); i++){
        d_state_next = simple_pendulum_forward_dynamics(state_vector[i], t_vector[i]);
        state_dot_vector.push_back(d_state_next);
        d_state_next.clear();
    }
    return state_dot_vector;
};
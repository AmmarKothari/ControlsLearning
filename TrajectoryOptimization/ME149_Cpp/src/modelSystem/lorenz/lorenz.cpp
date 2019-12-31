#include "lorenz.h"

Lorenz::Lorenz(float sigma_in, float rho_in, float beta_in){
    this-> sigma = sigma_in;
    this-> rho = rho_in;
    this-> beta = beta_in;
};

state_type Lorenz::forward_dynamics(state_type &state, float t){
    state_type dx_next;
    // dx_next = lorenz_forward_dynamics(state, t, sigma, rho, beta);
    return dx_next;
};

state_type lorenz_forward_dynamics(state_type &state, float t, float sigma, float rho, float beta){
    // state = [x, y, z]
    state_type dx_next = {0.0, 0.0, 0.0};
    dx_next[0] = sigma * (state[1] - state[0]);
    dx_next[1] = state[0] * (rho - state[2]) - state[1];
    dx_next[2] = state[0] * state[1] - beta * state[2];
    return dx_next;
};
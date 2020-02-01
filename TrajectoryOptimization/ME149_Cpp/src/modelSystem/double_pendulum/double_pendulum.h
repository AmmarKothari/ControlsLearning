#pragma once

#include <vector>
#include "../dynamics_model.h"
#include <cmath>

struct Link{
    float length{1.0};
    float mass{1.0}; 
    float damping{0.0};
};

class DoublePendulum : public DynamicsModel{
    public:
        Link l1;
        Link l2;
        float g;  // gravity
        state_type state;

        int num_states = 4;

        DoublePendulum(Link, Link, float);
        DoublePendulum();

        state_type forward_dynamics(state_type &, u_type &, float);

        state_vector_type assemble_full_state(state_vector_type &);
};
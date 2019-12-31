#include "dynamics_model.h"

state_type DynamicsModel::forward_dynamics(state_type &, float) {
    state_type dx;
    return dx;    
}

state_vector_type DynamicsModel::forward_dynamics_vectorized(state_vector_type &, time_vector_type) {
        state_vector_type dx_vector;
        return dx_vector;
}

void DynamicsModel::inverse_dynamics(){}



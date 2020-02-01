#include "dynamics_model.h"

state_type DynamicsModel::forward_dynamics(state_type &, u_type &, float) {
    state_type dx;
    return dx;    
}

state_vector_type DynamicsModel::forward_dynamics_vectorized(state_vector_type &x, u_vector_type &u, time_vector_type &time) {
        state_vector_type dx_vector;
        state_type dx_current;
        for (size_t i=0; i<x.size(); i++){
            dx_current = this->forward_dynamics(x[i], u[i], time[i]);
            dx_vector.push_back(dx_current);
            dx_current.clear();
        }
        return dx_vector;
}

void DynamicsModel::inverse_dynamics(){}



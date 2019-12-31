#include "euler_method_sim.h"

// void euler_method_sim(state_type dyn_func(state_type &x, float t), 
void euler_method_sim(std::function<state_type (state_type &x, float t)> dyn_func,
                    float t_span[], state_type x0, float t_max_step,
                    std::vector<state_type> &z_grid, std::vector<float> &t_grid){
    t_grid = arange(t_span[0], t_span[1], t_max_step);
    
    state_type next_state;
    state_type current_state;
    state_type current_state_derivative;

    size_t num_states = x0.size();
    
    z_grid.push_back(x0);  // Add the first state
    next_state.assign(num_states, 0.0);  // fill with 0.0 equal to the state size
    std::vector<float>::iterator t_current;
    for (t_current = t_grid.begin() + 1; t_current != t_grid.end(); t_current++){
        current_state = z_grid.back();
        current_state_derivative = dyn_func(current_state, *t_current);
        for (int i=0; i<num_states; i++){
            next_state[i] = current_state[i] + current_state_derivative[i] * t_max_step;
        }
        z_grid.push_back(next_state);
    }
}

// Using a dynamics model!
void euler_method_sim(DynamicsModel *dynamics_model, 
                    float t_span[], state_type x0, float t_max_step,
                    std::vector<state_type> &z_grid, std::vector<float> &t_grid){
    t_grid = arange(t_span[0], t_span[1], t_max_step);
    
    state_type next_state;
    state_type current_state;
    state_type current_state_derivative;

    size_t num_states = x0.size();
    
    z_grid.push_back(x0);  // Add the first state
    next_state.assign(num_states, 0.0);  // fill with 0.0 equal to the state size
    current_state = z_grid.back();
    std::vector<float>::iterator t_current;
    for (t_current = t_grid.begin() + 1; t_current != t_grid.end(); t_current++){
        current_state_derivative = dynamics_model->forward_dynamics(current_state, *t_current);
        for (int i=0; i<num_states; i++){
            next_state[i] = current_state[i] + current_state_derivative[i] * t_max_step;
        }
        z_grid.push_back(next_state);
        current_state = next_state;
    }
}
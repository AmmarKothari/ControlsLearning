#include "../modelSystem/dynamics_model.h"

#typedef simulation_func_type std::any;

void run_simulation(dyn_func_type dyn_func,
                t_vector_type t_grid, 
                state_type z_init,
                simulation_func_type sim_func):

    

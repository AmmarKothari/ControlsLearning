#ifndef _GUARD_EULER_METHOD_SIM
#define _GUARD_EULER_METHOD_SIM

#include "../../modelSystem/dynamics_model.h"
#include "../utils.h"

void euler_method_sim(std::function<state_type (state_type &, float)> dyn_func, float [], state_type, float,std::vector<state_type> &, std::vector<float> &);

void euler_method_sim(DynamicsModel *, float [], state_type, float, std::vector<state_type> &, std::vector<float> &);

#endif
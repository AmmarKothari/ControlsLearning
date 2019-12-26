#ifndef _GUARD_MODEL_DYNAMICS
#define _GUARD_MODEL_DYNAMICS
#include <vector>

typedef std::vector<float> state_type;
typedef std::function<state_type(state_type &, float)> dynamics_func_type;

class DynamicsModel{
    public:
        state_type forward_dynamics(state_type &x, float t);
        state_type inverse_dynamics();
};

#endif
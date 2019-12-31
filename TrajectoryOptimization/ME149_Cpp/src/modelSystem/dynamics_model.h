#ifndef _GUARD_DYNAMICS_MODEL
#define _GUARD_DYNAMICS_MODEL
#include <vector>

typedef std::vector<float> state_type;
typedef std::vector<float> time_vector_type;
typedef std::vector<state_type> state_vector_type;
typedef std::function<state_type(state_type &, float)> dynamics_func_type;

class DynamicsModel{
    public:
        virtual state_type forward_dynamics(state_type &, float);
        virtual void inverse_dynamics();
        
        state_vector_type forward_dynamics_vectorized(state_vector_type &, time_vector_type);

        int num_states; // Number of states
};

#endif
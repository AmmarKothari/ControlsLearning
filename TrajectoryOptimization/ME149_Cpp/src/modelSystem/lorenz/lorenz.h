#include <vector>
#include "../dynamics_model.h"
#include <cmath>

class Lorenz : public DynamicsModel{
    public:
        float sigma;
        float rho; 
        float beta;

        int num_states = 3;

        Lorenz(float sigma, float rhoa, float beta);
        Lorenz() : Lorenz(1.0, 1.0, 0.0){};

        state_type forward_dynamics(state_type &, float);

};

state_type lorenz_forward_dynamics(state_type &, float, float, float, float);
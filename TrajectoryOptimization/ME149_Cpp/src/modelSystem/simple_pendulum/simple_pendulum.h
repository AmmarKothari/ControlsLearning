#include <vector>
#include "../dynamics_model.h"
#include <cmath>

class SimplePendulum : public DynamicsModel{
    public:
        float length;
        float mass; 
        float damping;
        state_type state;

        SimplePendulum(float length_in, float mass_in, float damping_in);
        SimplePendulum(float length_in, float mass_in) : SimplePendulum(length_in, mass_in, 0.0){};
        SimplePendulum(float length_in) : SimplePendulum(length_in, 1.0, 0.0){};
        SimplePendulum() : SimplePendulum(1.0, 1.0, 0.0){};

        state_type forward_dynamics(state_type &, float);

};

state_type simple_pendulum_forward_dynamics(state_type &, float);
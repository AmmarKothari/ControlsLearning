#include <vector>
#include "../dynamics_model.cpp"

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

        state_type SimplePendulum::forward_dynamics(state_type, state_type, float);

};
#include <vector>

typedef std::vector<float> state_type;
class DynamicsModel{
    public:
        virtual state_type forward_dynamics(state_type &x ,state_type &dx ,float t );
        virtual state_type inverse_dynamics();
};
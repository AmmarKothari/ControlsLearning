#ifndef MotorEncoder_h
#define MotorEncoder_h

#include "Arduino.h"
#include "math.h"
#include "timer.h"
#include "velocity_history.h"

#define HISTORY_LEN 100

class MotorEncoder{
    public:
        MotorEncoder(int, int);
        void setup(void (*)(void));
        float get_angular_position();
        float get_angular_velocity();
        int get_velocity_history_length();
        volatile int pos;
        void _count_a(void);
    private:
        int _pin_a;
        int _pin_b;
        volatile int _state_a_prev = LOW;
        volatile int _state_a = LOW;
        volatile int _state_b = LOW;
        void _set_state_b();
        void _read_state_b();
        void _update_position_history(int, int);
        VelocityHistory* velocity_history;
        void(*ISR_callback)();
};


#endif // MotorEncoder_h

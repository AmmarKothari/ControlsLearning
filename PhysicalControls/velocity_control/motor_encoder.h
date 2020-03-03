#ifndef MotorEncoder_h
#define MotorEncoder_h

#include "Arduino.h"


class MotorEncoder{
    public:
        MotorEncoder(int, int);
        void setup();
        float get_angular_position();
    private:
        int _pin_a;
        int _pin_b;
        volatile int _state_a_prev = LOW;
        volatile int _state_a = LOW;
        volatile int _state_b = LOW;
        void _count_a();
        void _set_state_b();

}


#endif // MotorEncoder_h
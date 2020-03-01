#ifndef SimpleMotorControl_h
#define SimpleMotorControl_h

#include "Arduino.h"


class SimpleMotorControl{
    public:
        SimpleMotorControl(int in_enable_pin, int in_direction_pin_0, int in_direction_pin_1);
        int delay_between_writes_ms;
        void spin_motor_forward(int);
        void spin_motor_backward(int);
        void spin_motor(int);
        void setup();
    private:
        int enable_pin;
        int direction_pin_0;
        int direction_pin_1;
        int encoder_A_pin;
        int encoder_B_pin;
        int clip_speed(int);
};


#endif // SimpleMotorControl_h
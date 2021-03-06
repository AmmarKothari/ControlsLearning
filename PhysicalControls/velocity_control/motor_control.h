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
        int _enable_pin;
        int _direction_pin_0;
        int _direction_pin_1;
        int _clip_speed(int);
};


#endif // SimpleMotorControl_h
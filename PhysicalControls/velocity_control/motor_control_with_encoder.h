#ifndef MotorControlWithEncoder_h
#define MotorControlWithEncoder_h

#include "Arduino.h"
#include "motor_control.h"
#include "motor_encoder.h"

class MotorControlWithEncoder{
    public:
        MotorControlWithEncoder(int, int, int, int, int);
        void setup(void (*ISR_callback)(void));
        SimpleMotorControl* _motor;
        MotorEncoder* _encoder;

    private:
        void _update_position_history(int, int);
};

#endif // MotorControlWithEncoder_h

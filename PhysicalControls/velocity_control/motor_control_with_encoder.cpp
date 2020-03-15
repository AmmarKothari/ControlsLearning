#include "Arduino.h"
#include "motor_control_with_encoder.h"

MotorControlWithEncoder::MotorControlWithEncoder(int enable_pin, int direction_pin_0, int direction_pin_1, int encoder_pin_a, int encoder_pin_b){
    _motor = new SimpleMotorControl(enable_pin, direction_pin_0, direction_pin_1);
    _encoder = new MotorEncoder(encoder_pin_a, encoder_pin_b);
};

void MotorControlWithEncoder::setup(void (*ISR_callback)(void)){
    _motor->setup();
    _encoder->setup(ISR_callback);
//    _encoder->setup([this]{_encoder->_count_a;});
};

#include "Arduino.h"
#include "motor_encoder.h"

// Remember to disable to the interrupt flag if doing something to the calculations.

MotorEncoder::MotorEncoder(int in_pin_a, int in_pin_b){
    _pin_a = in_pin_a;
    _pin_b = in_pin_b;
}

void MotorEncoder::setup(){
    pinMode (_pin_a, INPUT);
    pinMode (_pin_b, INPUT);
    attachInterrupt(digitalPintoInterrupt(_pin_a), _count_a, CHANGE);
    attachInterrupt(digitalPintoInterrupt(_pin_b), _set_state_b, FALLING);
}

void MotorEncoder::_set_state_b(){
    _state_b = digitalRead(_pin_b);
}

void MotorEncoder::_count_a(){
// Count changes in encoder ticks
    noInterrupts();
    _state_a = digitalRead(_pin_a);
    if ((_state_a_prev == LOW) && (_state_a == HIGH)){
        if (state_b) == LOW {
            // Rotating backward
            pos--;
        }
        else {
            // Rotating forward
            pos++;
        }
    }
    interrupts();
}

float MotorEncoder::get_angular_position(){
    //Convert ticks to angular position
}

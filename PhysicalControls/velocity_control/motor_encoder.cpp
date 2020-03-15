#include "Arduino.h"
#include "motor_encoder.h"
#include "velocity_history.h"
#define _USE_MATH_DEFINES
#include "math.h"

#define TICKS_TO_REVOLUTION 1024
#define LOOK_BACK_VELOCITY_CALCULATION_TIME 1000  // 1 s

// Remember to disable to the interrupt flag if doing something to the calculations.

MotorEncoder::MotorEncoder(int in_pin_a, int in_pin_b){
    _pin_a = in_pin_a;
    _pin_b = in_pin_b;
    velocity_history = new VelocityHistory(2, HISTORY_LEN);
}

void MotorEncoder::setup(void (*ISR_callback)(void)){
    pinMode (_pin_a, INPUT);
    pinMode (_pin_b, INPUT);
//    attachInterrupt(digitalPinToInterrupt(_pin_a), _count_a, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_pin_a), ISR_callback, CHANGE);
//    attachInterrupt(digitalPinToInterrupt(_pin_b), _read_state_b, FALLING);
}

void MotorEncoder::_read_state_b(){
    _state_b = digitalRead(_pin_b);
}

void MotorEncoder::_count_a(void){
// Count changes in encoder ticks
    noInterrupts();
    _state_a = digitalRead(_pin_a);
    if ((_state_a_prev == LOW) && (_state_a == HIGH)){
        if (_state_b == LOW) {
            // Rotating backward
            pos--;
        }
        else {
            // Rotating forward
            pos++;
        }
    }
    _update_position_history(millis(), pos);
    interrupts();
}

float MotorEncoder::get_angular_position(){
    //Convert ticks to angular position
    noInterrupts();
    float _pos = pos;
    interrupts();
    float unwrapped_angular_position = float(_pos)/TICKS_TO_REVOLUTION;
    float angular_position = remainder(unwrapped_angular_position, 2*M_PI);
    return angular_position;
}


int MotorEncoder::get_velocity_history_length(){
    return velocity_history->get_length();
}

void MotorEncoder::_update_position_history(int time_milli, int pos){
	std::vector<int> current_measurement = {time_milli, pos};
    velocity_history->update(current_measurement);
}

float MotorEncoder::get_angular_velocity(){
    return velocity_history->avg_velocity_over_time(LOOK_BACK_VELOCITY_CALCULATION_TIME);
}

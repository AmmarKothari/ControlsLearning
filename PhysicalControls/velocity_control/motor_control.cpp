#include "Arduino.h"
#include "motor_control.h"

SimpleMotorControl::SimpleMotorControl(int in_enable_pin, int in_direction_pin_0, int in_direction_pin_1){
    _enable_pin = in_enable_pin;
    _direction_pin_0 = in_direction_pin_0;
    _direction_pin_1 = in_direction_pin_1;
    delay_between_writes_ms = 500;
};

void SimpleMotorControl::setup(){
    pinMode(_enable_pin, OUTPUT);
    pinMode(_direction_pin_0, OUTPUT);
    pinMode(_direction_pin_1, OUTPUT);
};

void SimpleMotorControl::spin_motor(int in_speed){
    if (in_speed > 0){
        spin_motor_forward(in_speed);
    }
    else{
        spin_motor_backward(in_speed);
    }
};

void SimpleMotorControl::spin_motor_forward(int in_speed){
  int speed = _clip_speed(in_speed);
  analogWrite(_enable_pin, speed);
  digitalWrite(_direction_pin_0, HIGH);
  digitalWrite(_direction_pin_1, LOW);
  // How to get around this delay?
  // Last time a write occurred otherwise skip
  // Or if value changed
  delay(delay_between_writes_ms);
};

void SimpleMotorControl::spin_motor_backward(int in_speed){
  int speed = _clip_speed(in_speed);
  analogWrite(_enable_pin, speed);
  digitalWrite(_direction_pin_0, LOW);
  digitalWrite(_direction_pin_1, HIGH);
  delay(delay_between_writes_ms);
};

int SimpleMotorControl::_clip_speed(int in_speed){
  int speed;
  if (in_speed < 0){
    speed = 0;
  }
  else if (in_speed > 255){
    speed = 255;
  }
  else{
    speed = in_speed;
  }
  return speed;
};

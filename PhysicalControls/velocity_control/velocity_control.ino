
#include <IntervalTimer.h>
#include "motor_control.h"

int motor_pin_1 = 0;
int motor_pin_2 = 1;
int enable_A = 23;
int led_pin = 13;
int encoder_A = 5;
int encoder_B = 4;

int encoder_pos = 0;

int SERIAL_BAUD_RATE = 9600;
SimpleMotorControl motor_1(enable_A, motor_pin_1, motor_pin_2);
IntervalTimer response_timer;
String inString;

void blink_light(int pin){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

void empty_status(){
    Serial.print("Uptime (s): ");
    Serial.println(millis() / 1000);
}


void setup_motor(){
  pinMode(led_pin, OUTPUT);

  pinMode(encoder_A, OUTPUT);
  pinMode(encoder_B, OUTPUT);
}

void setup_serial(){
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  response_timer.begin(empty_status, 1*1000000);
}

void setup() {
    motor_1.setup();
    setup_serial();
}

void loop() {
    if (Serial.available() > 0) // Waiting for information
    {
        inString = Serial.readString();
        inString.trim();
        if (inString.startsWith("set_speed")){
            // extract value and send to string
            int separate_at = inString.lastIndexOf(':');
            String in_motor_speed_string = inString.substring(separate_at+1);
            int in_motor_speed = int(in_motor_speed_string.toInt());
            motor_1.spin_motor(in_motor_speed);
            Serial.print("Setting motor speed to ");
            Serial.println(in_motor_speed);
        }
    }
}

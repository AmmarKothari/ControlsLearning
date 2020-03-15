CPP = /Users/ammarkothari/Downloads/Teensyduino.app/Contents/Java/hardware/tools/arm/bin/arm-none-eabi-g++

CPP_FLAGS = -std=gnu++14 -fno-exceptions -Wall
AVR_FLAGS = -felide-constructors  -fno-rtti  -g -Os -mcpu=cortex-m4 -mthumb -MMD -DF_CPU=48000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE -D__MK20DX256__ -DARDUINO=10805 -DTEENSYDUINO=144

TEENSYDUINO_SRC = /Users/ammarkothari/Downloads/Teensyduino.app/
ARDUINO_SRC = /Users/ammarkothari/Documents/Arduino/libraries/

INCLUDE_LIBS  = -I $(TEENSYDUINO_SRC)Contents/Java/hardware/teensy/avr/cores/teensy3
INCLUDE_LIBS += -I $(TEENSYDUINO_SRC)Contents/Java/hardware/teensy/avr/cores/teensy3/avr
INCLUDE_LIBS += -I $(ARDUINO_SRC)ArduinoSTL/src
INCLUDE_LIBS += -I $(ARDUINO_SRC)arduino-timer/src

all: motor_control.o motor_encoder.o

motor_encoder.o: motor_encoder.cpp
	$(CPP) $(CPP_FLAGS) $(AVR_FLAGS) $(INCLUDE_LIBS) -c -o $@ $<

velocity_history.o: velocity_history.cpp
	$(CPP) $(CPP_FLAGS) $(AVR_FLAGS) $(INCLUDE_LIBS) -c -o $@ $<

motor_control.o: motor_control.cpp
	$(CPP) $(CPP_FLAGS) $(AVR_FLAGS) $(INCLUDE_LIBS) -c -o $@ $<
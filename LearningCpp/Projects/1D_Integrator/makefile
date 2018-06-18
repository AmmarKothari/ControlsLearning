CC=g++
CFLAGS=-c -Wall
CXXFLAGS=-std=c++11 -Wall -pedantic


all: main

main: main.cpp dynamics.o PDcontroller.o

PDcontroller.o: PDcontroller.cpp

dynamics.o: dynamics.cpp


clean:
	rm main dynamics.o PDcontroller.o
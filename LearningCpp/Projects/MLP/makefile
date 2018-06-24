CC=g++
CFLAGS=-c -Wall
CXXFLAGS=-std=c++11 -Wall -pedantic


all: main test_sigmoid

test_sigmoid: test_sigmoid.cpp MLP.o

main: main.cpp MLP.o

MLP.o: MLP.cpp




clean:
	rm main MLP.o
CC=g++
CFLAGS=-c -Wall
CXXFLAGS=-std=c++11 -Wall -pedantic
INCLUDE_DIRS=/usr/local/include/eigen3


all: main

# test_sigmoid: test_sigmoid.cpp MLP.o

# main: main.cpp MLP.o
main: main.cpp
	$(CC) $(CXXFLAGS) -o main -I $(INCLUDE_DIRS) main.cpp
# MLP.o: MLP.cpp




clean:
	rm main
	# rm main MLP.o
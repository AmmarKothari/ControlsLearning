CXX = g++
CXXFLAGS = -std=c++0x -Wall

# OBJECTS = quaternion.o quaternion_basic_test.cpp

quaternion: quaternion.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

quaternion_basic_test: quaternion_basic_test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

all: quaternion quaternion_basic_test

# $(OBJECTS): quaternion.cpp
CXX = g++ -v
CXXFLAGS = -std=c++0x -Wall
# CXXFLAGS = -O

# OBJECTS = quaternion.o quaternion_basic_test.cpp

# quaternion.o: quaternion.cpp quaternion.h

quaternion.o: quaternion.cpp
	$(CXX) $(CXXFLAGS) -c quaternion.cpp

quaternion_basic_test.o: quaternion_basic_test.cpp
	$(CXX) $(CXXFLAGS) -c quaternion_basic_test.cpp color.h

quaternion_basic_test: quaternion_basic_test.o quaternion.o
	$(CXX) $(CXXFLAGS) -o quaternion_basic_test quaternion_basic_test.o quaternion.o


# quaternion:
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# quaternion_basic_test:
# 	$(CXX) $(CXXFLAGS) -o $@ $^	

# quaternion_basic_test.o: quaternion_basic_test.cpp quaternion.h
# 	$(CXX) $(CXXFLAGS) -o $@ $^

# quaternion_basic_test: quaternion_basic_test.o

all: quaternion_basic_test

# $(OBJECTS): quaternion.cpp
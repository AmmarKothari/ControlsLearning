CXX = g++
CC = g++

all:		Problem1_test

Problem1.o: Problem1.cc

Problem1_test.o: Problem1.o Problem1_test.cc test.o

Problem1_test: Problem1.o

test: test.cc test.h

test:		all
	./Problem1_test < Problem1_tests.txt

clobber:
	rm -f *.o core *.exe Problem1 Problem1_test

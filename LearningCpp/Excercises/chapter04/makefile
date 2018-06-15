CXX = g++ -std=c++11
CC = g++

all:		ex4_2 ex4_4 ex4_5

ex4_5.o:				ex4_5.cc ex4_utils.h

ex4_utils.o:		ex4_utils.cc ex4_utils.h

ex4_5:					ex4_5.o ex4_utils.o


test:		all
	./ex4_2
	./ex4_4
	./ex4_5 < ex4_5_testinput.txt

clobber:
	rm -f *.o *.exe ex4_2 ex4_4 ex4_5

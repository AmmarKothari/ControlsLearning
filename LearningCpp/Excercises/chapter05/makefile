CXX = g++
CC = g++

all:		ex5_2_vectors

Student_info_vectors.o: Student_info_vectors.cc Student_info_vectors.h

grade_vectors.o: grade_vectors.cc grade_vectors.h Student_info_vectors.h

median_vectors.o: median_vectors.cc median_vectors.h

ex5_2_vectors.o:	ex5_2_vectors.cc median_vectors.h grade_vectors.h Student_info_vectors.h

ex5_2_vectors:		ex5_2_vectors.o grade_vectors.o median_vectors.o Student_info_vectors.o

test:
	./ex5_2_vectors <../Data/grades

clobber:
	rm -f *.o *.exe core ex5_2_vectors

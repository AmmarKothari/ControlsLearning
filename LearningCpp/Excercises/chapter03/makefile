CXX = g++
CC = g++

all:		ex3_2 ex3_3 ex3_4 ex3_5

test:		all
	./ex3_2 < ../Data/quartiles_data
	./ex3_3 < ../Data/test_string.txt
	./ex3_4 < ../Data/test_string.txt
	./ex3_5 < ../Data/multiple_student_grades.txt

clobber:
	rm -f *.o *.exe ex3_2 ex3_3 ex3_4 ex3_5

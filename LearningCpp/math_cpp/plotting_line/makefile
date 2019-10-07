CXX = g++
CC = g++

test: clobber write_data
	./write_data && python plot_line.py

all: write_data

clobber:
	rm -f write_data input_file.csv

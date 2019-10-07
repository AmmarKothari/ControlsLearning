CXX = g++
CC = g++

test: clobber load_img
	./load_img && python plot_line.py

all: load_img

load_img: load_img.cpp
	CC loag_img.cpp -o load_img

clobber:
	rm -f load_img input_file.csv

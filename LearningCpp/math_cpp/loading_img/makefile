CXX = g++
CC = g++

# test: clobber load_img
# 	./load_img && python plot_line.py

test: clean load_img
	./load_img

all: load_img

load_img: load_img.cpp
# 	$(CC) load_img.cpp -o load_img -I/opt/X11/include
	g++ -o load_img load_img.cpp -O2 -lm -lpthread -lX11 -I/usr/X11R6/include -L/usr/X11R6/lib
clean:
	rm -f load_img

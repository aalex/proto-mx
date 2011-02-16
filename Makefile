all: run

run: main.cpp
	gcc -Wall -g `pkg-config clutter-1.0 mx-1.0 --cflags --libs` -lm main.cpp -o run
clean:
	rm -f run *.o


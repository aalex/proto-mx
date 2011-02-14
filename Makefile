all: run

run: main.c
	gcc -Wall -g `pkg-config clutter-1.0 mx-1.0 --cflags --libs` -lm main.c -o run
clean:
	rm -f run *.o


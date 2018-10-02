CC = g++
CFLAGS = -g -Wall -std=c++11 -O3 -Wl,--stack,419304

default: path_test

path_test:
	$(CC) $(CFLAGS) main.cpp pathfinders.cpp -o $@

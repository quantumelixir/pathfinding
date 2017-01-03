CC = g++
CFLAGS = -g -Wall -std=c++11 -O3
INCLUDES = -I/usr/local/Cellar/boost/1.62.0/include/
LIBFLAGS = -L/usr/local/Cellar/boost/1.62.0/lib
LIBS = -lboost_system -lboost_random

default: a.out

a.out:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBFLAGS) $(LIBS) main.cpp pathfinders.cpp -o $@

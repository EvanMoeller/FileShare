CFLAGS = -Wall -Wextra -DDEBUG -g -pedantic -std=c++14
SHELL := /bin/bash

all: Predict.o main.o
	g++ $(CFLAGS) Predict.o main.o -o predictors

main.o: main.cpp
	g++ $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o predictors

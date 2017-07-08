CC=g++
CFLAGS= -Wall -Wpedantic -Wextra -g -I'include' -std=c++11
SRC_DIR=src
SRCS := $(shell find $(SRC_DIR)/* -maxdepth 0 -name '*.cpp')

all: dirs
	$(CC) $(CFLAGS) $(SRCS) main.cpp -o bin/main -lm

opt: dirs
	$(CC) $(CFLAGS) $(SRCS) main.cpp -o bin/main -lm -D OPT

test: all
	./bin/main testing/cases/random_test_1

valgrind: all
	valgrind ./bin/main

zip: all
	zip -r SequencePairs include/* src/* main.cpp testing/* Readme.md Makefile 

dirs: 
	mkdir -p bin


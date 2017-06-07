CC=g++
CFLAGS= -Wall -Wpedantic -Werror -g -I'include' -std=c++11
SRC_DIR=src
SRCS := $(shell find $(SRC_DIR)/* -maxdepth 0 -name '*.cpp')

all: dirs
	$(CC) $(CFLAGS) $(SRCS) main.cpp -o bin/main -lm

run: all
	./bin/main

test: all
	python ./testing/assert_testcases.py

dirs: 
	mkdir -p bin


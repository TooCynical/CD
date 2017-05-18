CC=g++
CFLAGS= -Wall -g -D_POSIX_SOURCE -D_GNU_SOURCE -I'include' -std=c++11
SRC_DIR=src
SRCS := $(shell find $(SRC_DIR)/* -maxdepth 0 -name '*.cpp')

all: dirs
	$(CC) $(CFLAGS) $(SRCS) main.cpp -o bin/main -lm

run: all
	./bin/main

dirs: 
	mkdir -p bin


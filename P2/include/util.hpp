/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * util.hpp
 *
 * Basic utility functions and constants.
 */

#ifndef UTIL_H
#define UTIL_H

#define MAX_RECT_DIM    1000000000
#define MAX_RECTANGLES  32
#define MAX_ITERATIONS  100000000

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <climits>
#include <stdint.h>


/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

/* Free a 2D int array of given width. */
Result Free2DArray(unsigned long long** array, unsigned long long n);

/* Factorial function. */
unsigned long long Fact(unsigned long long n);

/* Print a vector. */
void print_vector(std::vector<unsigned long long> v);

/* Subtract two unsgined ints throwing an exception upon underflow. */
unsigned long long subtract(unsigned long long a, unsigned long long b);

/* Return whether a < b <= c. */
bool between_left(unsigned long long a, unsigned long long b, unsigned long long c);
/* Return whether a <= b < c. */
bool between_right(unsigned long long a, unsigned long long b, unsigned long long c);



#endif

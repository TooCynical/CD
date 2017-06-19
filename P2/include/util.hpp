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
Result Free2DArray(unsigned** array, unsigned n);

/* Factorial function. */
unsigned Fact(unsigned n);

/* Print a vector. */
void print_vector(std::vector<unsigned> v);

/* Subtract two unsgined ints throwing an exception upon underflow. */
unsigned subtract(unsigned a, unsigned b);

/* Return whether a < b <= c. */
bool between_left(unsigned a, unsigned b, unsigned c);
/* Return whether a <= b < c. */
bool between_right(unsigned a, unsigned b, unsigned c);



#endif

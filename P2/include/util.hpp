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

#define MAX_RECT_DIM    1000
#define MAX_RECTANGLES  32
#define MAX_ITERATIONS  100000000 

#include <vector>
#include <stdlib.h>
#include <iostream>

/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

/* Free a 2D int array of given width. */
Result Free2DArray(unsigned int** array, size_t n);

/* Factorial function. */
size_t Fact(size_t n);

/* Print a vector. */
void print_vector(std::vector<size_t> v);

/* Subtract two unsgined ints and print a warning if underflow occurs. */
size_t subtract(size_t a, size_t b);

#endif

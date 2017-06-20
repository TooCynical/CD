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

#ifndef UTIL_HPP
#define UTIL_HPP

#define MAX_RECT_DIM    2147483649 // 2^31 + 1, maximum value of a signed int32.
#define MAX_RECTANGLES  100

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <climits>

#include <stdint.h>

/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

/* Free a 2D int array of given width. */
Result Free2DArray(uint64_t** array, uint64_t n);

/* Factorial function. */
uint64_t Fact(uint64_t n);

/* Print a vector. */
void print_vector(std::vector<uint64_t> v);

/* Subtract two unsgined ints throwing an exception upon underflow. */
uint64_t subtract(uint64_t a, uint64_t b);

/* Return whether a < b <= c. */
bool between_left(uint64_t a, uint64_t b, uint64_t c);
/* Return whether a <= b < c. */
bool between_right(uint64_t a, uint64_t b, uint64_t c);

#endif

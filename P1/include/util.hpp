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

#define BITSET_SIZE 64
#define MAX_COORD 999
#define MAX_TERMINALS 64
#define DOUBLE_INPUT_COORDS 1

#include <stdlib.h>

/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

/* Free a 2D int array of given width. */
Result Free2DArray(int** array, int n);

#endif

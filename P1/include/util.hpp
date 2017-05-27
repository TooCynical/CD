#ifndef UTIL_H
#define UTIL_H

#define BITSET_SIZE 20
#define MAX_COORD 999
#define MAX_TERMINALS 20
#define DOUBLE_INPUT_COORDS 1

#include <stdlib.h>

/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

Result Free2DArray(int** array, int n);

#endif

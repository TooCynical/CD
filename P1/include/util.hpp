#ifndef UTIL_H
#define UTIL_H

#define BITSET_SIZE 32

#include <stdlib.h>

/* The result type used instead of something unclear like return 0. */
typedef enum Result { FAIL, SUCCESS } Result; 

Result Free2DArray(int** array, int n);

#endif

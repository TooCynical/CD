/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * util.cpp
 *
 * Implementation of functionality described in util.hpp.
 */

#include "util.hpp"

Result Free2DArray(unsigned int** array, size_t n) {
    for (size_t i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}


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

Result Free2DArray(int** array, int n) {
    for (int i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}


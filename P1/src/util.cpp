#include "util.hpp"

/* Free a 2D int array of given width */
Result Free2DArray(int** array, int n) {
    for (int i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}
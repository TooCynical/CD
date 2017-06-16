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

using namespace std;

Result Free2DArray(unsigned int** array, size_t n) {
    for (size_t i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}

size_t Fact(size_t n) {
    size_t ret = 1;
    for(size_t i = 1; i <= n; i++)
        ret *= i;
    return ret;
}

void print_vector(vector<size_t> v) {
    for (auto const& c : v)
        cout << c << ' ';
    cout << endl;
}
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

size_t subtract(size_t a, size_t b) {
    if (a < b)
        cout << "Util: Warning subtract lead to underflow" << endl;
    return a - b;
}

bool between_right(size_t a, size_t b, size_t c) {
    return ((a <= b) && (b < c));
}

bool between_left(size_t a, size_t b, size_t c) {
    return ((a < b) && (b <= c));
}
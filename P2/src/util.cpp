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

Result Free2DArray(unsigned** array, unsigned n) {
    for (unsigned i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}

/* TODO: add exception. */
unsigned Fact(unsigned n) {
    unsigned ret = 1;
    for(unsigned i = 1; i <= n; i++)
        ret *= i;
    return ret;
}

void print_vector(vector<unsigned> v) {
    for (auto const& c : v)
        cout << c << ' ';
    cout << endl;
}

/* TODO: add exception. */
unsigned subtract(unsigned a, unsigned b) {
    if (a < b)
        cout << "Util: Warning subtract lead to underflow" << endl;
    return a - b;
}

bool between_right(unsigned a, unsigned b, unsigned c) {
    return ((a <= b) && (b < c));
}

bool between_left(unsigned a, unsigned b, unsigned c) {
    return ((a < b) && (b <= c));
}
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

Result Free2DArray(uint64_t** array, uint64_t n) {
    for (size_t i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}

uint64_t Fact(uint64_t n) {
    uint64_t max_uint64 = -1;
    uint64_t ret = 1;
    for(uint64_t i = 1; i <= n; i++) {
        if ((max_uint64 / i) <= ret) {
            throw overflow_error("Factorial does not fit in uint64!");
            return 0;
        }
        ret *= i;
    }
    return ret;
}

void print_vector(vector<uint64_t> v) {
    for (auto const& c : v)
        cout << c << ' ';
    cout << endl;
}

/* TODO: add exception. */
uint64_t subtract(uint64_t a, uint64_t b) {
    if (a < b)
        throw underflow_error("Subtraction lead to underflow!");
    return a - b;
}

bool between_right(uint64_t a, uint64_t b, uint64_t c) {
    return ((a <= b) && (b < c));
}

bool between_left(uint64_t a, uint64_t b, uint64_t c) {
    return ((a < b) && (b <= c));
}
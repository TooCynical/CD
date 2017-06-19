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

Result Free2DArray(unsigned long long** array, unsigned long long n) {
    for (unsigned long long i = 0; i < n; i++)
            free(array[i]);
    free(array);
    return SUCCESS;
}

/* TODO: add exception. */
unsigned long long Fact(unsigned long long n) {
    unsigned long long ret = 1;
    for(unsigned long long i = 1; i <= n; i++)
        ret *= i;
    return ret;
}

void print_vector(vector<unsigned long long> v) {
    for (auto const& c : v)
        cout << c << ' ';
    cout << endl;
}

/* TODO: add exception. */
unsigned long long subtract(unsigned long long a, unsigned long long b) {
    if (a < b)
        cout << "Util: Warning subtract lead to underflow" << endl;
    return a - b;
}

bool between_right(unsigned long long a, unsigned long long b, unsigned long long c) {
    return ((a <= b) && (b < c));
}

bool between_left(unsigned long long a, unsigned long long b, unsigned long long c) {
    return ((a < b) && (b <= c));
}
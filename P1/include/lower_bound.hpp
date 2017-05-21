#ifndef LOWERBOUND_HPP
#define LOWERBOUND_HPP

#include "util.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "label.hpp"
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <climits>

using namespace std;

/* Forward declaration */
class Label;
class Vertex;

/* Bounding box lower bound. */
int BBLowerBound(Label *l, Vertex **terminals, int n);

/* MST lower bound */
int MSTLowerBound(Label *l, Vertex **terminals, int n);
/* Compute the length of an MST on the given terminal set. */
int MST(bitset<BITSET_SIZE> I, Vertex **terminals, int n);

#endif
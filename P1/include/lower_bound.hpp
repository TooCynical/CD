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
#include <unordered_map>

using namespace std;

/* Forward declaration */
class Label;
class Vertex;
class Instance;

class LowerBoundComputator {
    private:
        Instance *_underlying_instance;

        /* Hash table containing MST(I) for a terminal set I, if 
         * it has been computed already. */
        unordered_map<bitset<BITSET_SIZE>, int> _MST_hash;

        /* Hash table containing d(I, R-I) for a terminal set I, if 
         * it has been computed already */
        unordered_map<bitset<BITSET_SIZE>, int> _distance_hash;

        
    public:
        /* Compute the distance d(I, R-I) between a terminal set and 
         * its complement. */
        int ComplementDistance(const bitset<BITSET_SIZE> &I);

        /* Constructor */
        LowerBoundComputator(Instance *inst);

        /* Compute the length of an MST on the given terminal set. */
        int MST(const bitset<BITSET_SIZE> &I);

        /* Bounding box lower bound. */
        int BBLowerBound(Label *l);

        /* MST lower bound */
        int MSTLowerBound(Label *l);
};


#endif
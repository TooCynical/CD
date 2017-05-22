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

        /* Compute the length of an MST on the given terminal set. */
        int MST(bitset<BITSET_SIZE> I);

    public:
        /* Constructor */
        LowerBoundComputator(Instance *inst);

        /* Bounding box lower bound. */
        int BBLowerBound(Label *l);

        /* MST lower bound */
        int MSTLowerBound(Label *l);
};


#endif
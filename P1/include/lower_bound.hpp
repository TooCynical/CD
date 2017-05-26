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
#include <set>
#include <utility>
#include <iterator>

using namespace std;

/* Forward declaration */
class Label;
class Vertex;
class Instance;

class BoundComputator {
    private:
        Instance *_underlying_instance;

        /* Hash table containing MST(I) for a terminal set I, if 
         * it has been computed already. */
        unordered_map<bitset<BITSET_SIZE>, int> _MST_hash;

        /* Hash table containing d(I, R-I) for a terminal set I, if 
         * it has been computed already */
        unordered_map<bitset<BITSET_SIZE>, pair<int, int> > _distance_hash;

        /* Hash containing the upper bound U(I) for a terminal set I. */
        // unordered_map<bitset<BITSET_SIZE>, int> _upper_bound_hash;
       
        /* Hash containing the upper bound U(I) and the set S(I) used to 
         * find U(I) for a terminal set I, represented as a pair of an 
         * integer (U(I)) and an ordered set of integers (S(I)), 
         * corresponding to the unique IDs of the vertices in S(I).
         * We refer to this pair as A(I). */
        unordered_map<bitset<BITSET_SIZE>, pair<int, bitset<BITSET_SIZE> > > 
        _upper_bound_hash;

        Result GetComplementDistance(const bitset<BITSET_SIZE> &I,
                                     int &ret_dist,
                                     int &ret_ind);

        /* Compute the distance d(I, R-I) between a terminal set and 
         * its complement. */
        Result ComplementDistance(const bitset<BITSET_SIZE> &I, 
                                  int &ret_dist,
                                  int &ret_ind);

        /* Compute the distance d(v, I-R) between a vertex and the complement
         * of the given bitset. */
        Result VertexComplementDistance(const bitset<BITSET_SIZE> &I,
                                        Vertex *v, 
                                        int &ret_dist, 
                                        int &ret_ind);

        /* Return whether I intersects S. */
        bool SetIntersectsTerminalSet(const bitset<BITSET_SIZE> &I, 
                                     set<int> &S);

    public:
        /* Return whether value > U(I), the local upper bound for I. */
        bool CompareToUpperBound(bitset<BITSET_SIZE> &I, int value);

        Result UpdateUpperBound(Label *l);
        Result MergeUpperBound(const bitset<BITSET_SIZE> &I,
                               const bitset<BITSET_SIZE> &J);

        /* Constructor */
        BoundComputator(Instance *inst);

        /* Compute the length of an MST on the given terminal set. */
        int MST(const bitset<BITSET_SIZE> &I);

        /* Bounding box lower bound for given label. */
        int BBLowerBound(Label *l);

        /* MST lower bound for given label. */
        int MSTLowerBound(Label *l);
};


#endif
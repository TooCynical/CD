/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * bound_computator.hpp
 *
 * BoundComputator is a class that, given a valid Instance object
 * representing an instance of the 3d rectilinear steiner tree problem
 * contains functionality to compute lower bounds and upper bounds
 * used in the Dijkstra-Steiner algorithm with future costs and pruning.
 * A LowerBoundOptions struct should be provided to specify which lower
 * bound(s) should be used.
 * In particular, it is capable of computing:
 *      - The length of an MST on a given terminal set
 *      - The bounding box lower bound BB(v, I) of a label.
 *      - The One-Tree lower bound MST(v, I) of a label.
 *
 *      - A local upper bound U(I) for a bitset I given a label
 *        (v, I).
 *      - A local upper bound U(I u J) given two bitsets I and J.
 *
 *      - The distance between a terminal set I and its complement.
 *      - The distance between a vertex v and the complement
 *        of a terminal set I.
 *      - The vertex / terminal attaining these minimum distances.
 *  
 * In order to efficiently make use of / look up previous computational
 * results, BoundComputator contains hash tables using bitsets
 * as keys, containing MST(I), A(I), d(I, R-I).
 *
 *
 * BoundComputator makes the same assumptions on the given instance
 * Solver does.
 * 
 *
 * Notation used:
 *      P           : The set of all permanently valued labels. 
 *      R           : The set of all terminals
 *      l(v,I)      : the (tentative) value of a label.
 *      LB(v, I)    : the (permanent) lower bound for a label.
 *      root        : The root terminal (always the first terminal
 *                    in the array _terminals in the given instance).
 *      U(I)        : The best local upper bound for the terminal set I.
 *      S(I)        : The set of terminals used to compute U(I) for a 
 *                    terminal set I.
 *      A(I)        : The pair (U(I), S(I)) for a terminal set I.
 */

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

/* Forward declarations */
class Label;
class Vertex;
class Instance;

struct LowerBoundOptions {
    bool _use_BB_lower_bound;
    bool _use_onetree_lower_bound;
};

class BoundComputator {
    private:
        /* The problem instance for which this BoundComputator should
         * compute bounds. */
        Instance *_underlying_instance;
        
        int _n_terminals;       // The amount of terminals in the 
                                // underlying instance.
        Vertex **_terminals;    // References to the terminals in 
                                // the underlying instance.

        bool _use_BB_lower_bound;           // Use Bounding Box lb?
        bool _use_onetree_lower_bound;      // Use 1-Tree lb?

        /* Hash table containing MST(R-I) for a terminal set I, if 
         * it has been computed already. */
        unordered_map<bitset<BITSET_SIZE>, int> _MST_hash;

        /* Hash table containing (d(I, R-I), i) for a terminal set I, if 
         * it has been computed already, where i is the index of the terminal
         * in R-I minimizing d(i, I). */
        unordered_map<bitset<BITSET_SIZE>, pair<int, int> > _distance_hash;
    
        /* Hash table containing pairs A(I) = (U(I), S(I)), where 
         * U(I) is the current upper bound for a terminals set I, and 
         * S(I) is the set of terminals used to find U(I). */
        unordered_map<bitset<BITSET_SIZE>, pair<int, bitset<BITSET_SIZE> > > 
        _upper_bound_hash;

        /* Find the distance d(I, R-I) between a terminal set and its
         * complement, as well as the index of the terminal i in R-I 
         * minimizing d(i, I) either by fetching these values from the
         * hash table if possible, or computing them. After a 
         * compputation the results are stored in _distance_hash. */
        Result GetComplementDistance(const bitset<BITSET_SIZE> &I,
                                     int &ret_dist,
                                     int &ret_ind);

        /* Compute the distance d(I, R-I) between a terminal set and its
         * complement, as well as the index of the terminal i in R-I 
         * minimizing d(i, I) in O(|I||R-I|). */
        Result ComplementDistance(const bitset<BITSET_SIZE> &I, 
                                  int &ret_dist,
                                  int &ret_ind);

        /* Compute the distance d(v, R-I) between a vertex and the 
         * complement ofthe given terminal set, as well as the index 
         * of the terminal i in R-I minimizing d(v, i) in O(|R-I|). */
        Result VertexComplementDistance(const bitset<BITSET_SIZE> &I,
                                        Vertex *v, 
                                        int &ret_dist, 
                                        int &ret_ind);

    public:
        /* Return l(v, I) > U(I), the local upper bound for I. */
        bool CompareToUpperBound(Label *l);

        /* Update the local upper bound U(I) given a label of the form 
         * (v, I) by setting 
         *     U(I) = min(U(I), l(v,I) + min(d(v, R-I), d(I, R-I))).
         * Also update S(I) appropriately. We assume here that (v,I) 
         * is the label minimizing l(v, I) + lb(v, I) among all labels
         * not in P. */
        Result UpdateUpperBound(Label *l);
        
        /* Update the local upper bound U(I u J) given two terminal sets
         * I and J with I \cap S(J) or J \cap S(I) empty by setting:
         *      U(I u J) = min(U(I u J), U(I) + U(J)).
         * Also update S(I u J) if needed by setting 
         *      S(I u J) = S(I) u S(J) - I - J. */
        Result MergeUpperBound(const bitset<BITSET_SIZE> &I,
                               const bitset<BITSET_SIZE> &J);

        /* Constructor */
        BoundComputator(Instance *inst, LowerBoundOptions *opts);

        /* Compute the length of an MST on the given terminal set using
         * Prim's algorithm running in O(|R|^2). */
        int MST(const bitset<BITSET_SIZE> &I);

        /* Compute the bounding box length BB({v} u I) for the given label in
         * O(|I| + 1) */
        int BBLowerBound(Label *l);

        /* Compute the 1-tree lower bound for the given label (v, I). 
         * The required value MST(I) is fetched from the hash table if 
         * possible, and computed otherwise. */
        int OneTreeLowerBound(Label *l);

        /* Return the lb(v, I) for the given label, where lb is 
         * a lower bound function determined by the values of 
         * _use_BB_lower_bound and _use_MST_lower_bound. */
        int GetLowerBound(Label *l);

        /* Getters / Setters. */
        Result SetUseBBLowerBound();
        Result SetUseOneTreeLowerBound();
};

#endif
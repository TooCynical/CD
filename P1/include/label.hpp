/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * label.hpp
 */

#ifndef LABEL_HPP
#define LABEL_HPP

#include "util.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "lower_bound.hpp"

#include <iostream>
#include <stdlib.h>
#include <bitset>

using namespace std;

/* Forward declarations. */
class Vertex;
class Label;
class Instance;

class Label {
    private:
        Vertex *_v;             // Vertex associated with this label.
        bitset<BITSET_SIZE> _I; // Terminal set associated with this label.
  
        int _l;                 // Tentative value for this label.
        bool _l_set;            // Does this label have a tentative value yet?
        
        int _lower_bound;       // Lower bound for the complement label.
        bool _lower_bound_set;  // Has lower bound been computed yet?

        bool _in_P;             // Is this label in P?


    public:
        bool _pruned = false;

        /* Constructor. */
        Label(Vertex *v, const bitset<BITSET_SIZE> &I);

        /* Getters. */
        int GetL() const;
        int GetLowerBound() const;
        bool IsLSet() const;
        bool IsLowerBoundSet() const;
        bool IsInP() const;

        /* Return a reference to the vertex associated with 
         * this label. */
        Vertex *GetVertex() const;

        /* Return the bitset representing the terminal set 
         * associated with this label. */
        const bitset<BITSET_SIZE> &GetBitset() const;

        /* Set a value for _l and set _l_set to true. */
        Result SetL(int l);
        
        /* Set a value for _lower_bound and set 
         * _lower_bound_set to true. */
        Result SetLowerBound(int l);

        /* Set _in_P to true. Return FAIL
         * if _in_P was already true. */
        Result SetInP();

        /* IO-functions for testing */
        void Print() const;
};

#endif
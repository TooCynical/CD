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

/* Forward declaration */
class Vertex;
class Label;
class Instance;

class Label {
    private:
        Vertex *_v;
        bitset<BITSET_SIZE> _I;
  
        int _l;                 // Tentative value for this label.
        bool _l_set;            // Does this label have a tentative value yet?
        
        int _lower_bound;       // Lower bound for the complement label.
        bool _lower_bound_set;  // Has lower bound been computed yet?

        bool _in_P;             // Is this label in P?

    public:
        /* Constructor / Destructor. */
        Label(Vertex *v, bitset<BITSET_SIZE> I);

        /* Getters / Setters */
        int GetL() const;
        int GetLowerBound() const;
        bool IsLSet() const;
        bool IsLowerBoundSet() const;
        bool IsInP() const;
        bool BeenInQ() const;
        Vertex *GetVertex() const;
        bitset<BITSET_SIZE> *GetBitset();

        Result SetL(int l);
        Result SetLowerBound(int l);
        Result SetInP();
        Result SetBeenInQ();

        /* IO-functions for testing */
        void Print();
};

/* Operator overloads required to use heaps of Label pointers.
 * Comparison of pointers is derived from comparison of the 
 * referenced labels, which is defined in label.cpp */
bool operator<(const Label& a, const Label& b);

struct LabelPointerComp {
    bool operator() (const Label* lhs, const Label* rhs) const {
        return *lhs < *rhs;
    }
};

#endif
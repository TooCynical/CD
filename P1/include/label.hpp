#ifndef LABEL_HPP
#define LABEL_HPP

#include "util.hpp"
#include "vertex.hpp"
#include <iostream>
#include <stdlib.h>
#include <bitset>

using namespace std;

/* Forward declaration */
class Vertex;

class Label {
    private:
        Vertex *_v;
        bool _vertex_is_set;
        bitset<BITSET_SIZE> _I;
        bool _terminalset_is_set;
    
        int _l;
        bool _l_set;
        bool _in_P;

    public:
        // Result SetVertex(Vertex* v);
        // Result SetTerminalSet(bitset<BITSET_SIZE> &I);
        Label(Vertex *v, bitset<BITSET_SIZE> I);

        Result SetL(int l);
        Vertex *GetVertex() const;
        bitset<BITSET_SIZE> *GetBitset();

        int GetL() const;
        bool IsLSet() const;
        bool IsInP() const;
        Result SetInP();


        void Print();
};

bool operator<(const Label& a, const Label& b);

struct LabelPointerComp {
    bool operator() (const Label* lhs, const Label* rhs) const
    {
        return *lhs < *rhs;
    }
};

#endif
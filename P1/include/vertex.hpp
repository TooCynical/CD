#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "util.hpp"
#include "label.hpp"

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <iterator>


using namespace std;

/* Forward declaration */
class Label;
class Vertex;
class Instance;

class Vertex {
    private:
        int _x, _y, _z;

        int _n_neigh;                   // Number of neighbours set.
        vector<Vertex*> _neigh;         // Neighbours of vertex in Hanan grid.

        int _n_labels;                  // Number of lables with l < infty.
        vector<Label*> _labels;         // References to labels with l < infty.

        bool _is_root;                  // Is this the root vertex?

        /* Hash table containing the index of a label in _labels, 
        * if it is indeed in _labels. */
        unordered_map<bitset<BITSET_SIZE>, int> _label_hash;   


    public:
        /* Constructor / Destructor */
        Vertex(int x, int y, int z);
        ~Vertex();
        
        Result AddNeighbour(Vertex* w);
        Result AddLabel(Label *l);
        bool HasCoords(int x, int y, int z);

        /* Getters / Setters */
        int GetX();
        int GetY();
        int GetZ();

        bool IsRoot() const;
        Result SetRoot();

        int GetNNeigh();
        vector<Vertex*> *GetNeigh();

        vector<Label*> *GetLabels();
        unordered_map<bitset<BITSET_SIZE>, int> *GetLabelHash();
        
        /* IO-functions for testing */
        void Print();
        void PrintNeighbours();
};

int RectDistance(Vertex *v, Vertex *w);

#endif
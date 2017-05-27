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
        int _id = -1;

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
        
        /* Add given vertex to the neighbours of this vertex and increase
         * the number of neighbours. */
        Result AddNeighbour(Vertex* w);

        /* Add given reference to label to _labels and add its index 
         * to the hash table for labels. */
        Result AddLabel(Label *l);

        /* Return whether the vertex is located at given coordinates. */
        bool HasCoords(int x, int y, int z) const;

        /* Getters / Setters */
        const int& GetX() const;
        const int& GetY() const;
        const int& GetZ() const;

        /* Return ID of this vertex. Print a warning if it was unset (<0). */
        const int& GetId() const;

        /* Set an ID for this vertex, which must be positive. Return FAIL
         * if an ID was already set. */
        Result SetId(int id);

        /* Return whether this vertex is the root. */
        bool IsRoot() const;

        /* Make this vertex the root. Return FAIL if it was already root. */
        Result SetRoot();

        /* Return the amount of neighbours of this vertex. */
        const int &GetNNeighbours() const;

        /* Return a reference to the neighbours of this vertex. */
        const vector<Vertex*> &GetNeighbours() const;

        /* Return a reference to the labels corresponding to this vertex. */
        const vector<Label*> &GetLabels() const;

        /* Find the label corresponding to the given bitset in the hash
         * table and return a reference to it. Return NULL if the label 
         * cannot be found. */
        Label* GetLabelByBitset(const bitset<BITSET_SIZE> &I) const;
        
        /* Operator overload that returns whether the IDs of the given
         * vertices are identical. */
        bool operator==(const Vertex& other);

        /* IO-functions for testing */
        void Print() const;
        void PrintNeighbours() const;
};

/* Return the rectilinear distance between two vertices. */
int RectDistance(Vertex *v, Vertex *w);

#endif
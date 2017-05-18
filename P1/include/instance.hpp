#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <stdlib.h>
#include <vector>
#include <set>

#include "vertex.hpp"
#include "util.hpp"

using namespace std;

class Instance {
    private:
        int _is_init;

        int _n_terminals;
        int _n_vertices;

        vector<Vertex*> _V;     // References to all vertices in lexicographical order.
        Vertex**** _hanan_grid; // References to all vertices, ordered according to
                                // the Hanan grid.
        Vertex** _terminals;    // References to all terminals.

        set<int> _x_values;
        set<int> _y_values;
        set<int> _z_values;
        int _nx;
        int _ny;
        int _nz;

        Result SetTerminals(int **term_locs);
        Result SetXYZ(int **term_locs);
        Result SetVertices();
        Result SetHananGrid();
        Result SetNeighbours();

    public:
        Instance(int n, int **term_locs);
        void PrintTerminals();
        void PrintVertices();

        vector<Vertex*> *GetV();
        Vertex **GetTerminals();
        int GetNTerminals();
        int GetNVertices();

};

#endif
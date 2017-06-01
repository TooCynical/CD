/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * instance.hpp
 *
 *
 * Instance is a class that, given an n x 3 array of integer
 * coordinates, ccomputes all necessary representations of the 
 * 3d rectilinear steiner tree problem with terminals given by
 * the entries of this array. In particular, Instance computes
 * the following on initialization:
 *      - A vector containing references to vertices with coordinates
 *        according to the given array (the terminals), as well as references
 *        to vertices with coordinates in the Hanan grid generated by the 
 *        terminals.
 *      - A 1D array of references to the terminals, in which the first entry
 *        is a reference to the terminal closest to the center of gravity.
 *      - A 3D array of references to the above vertices ordered according
 *        to the Hanan grid positions.
 *
 * Furthermore, Instance will provide each of the vertices it produces with
 * a unique integer ID, as well as a all of the vertex's neighbours in 
 * the Hanan grid. Detailed explanations of each of Instance's members and 
 * methods are found in the declarations below.
 *
 *
 * Instance assumes, but does not check, that:
 *      - All given integer coordinates are between 0-2000
 *      - None of the given triples of integer coordinates are duplicate.
 */

#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <stdlib.h>
#include <vector>
#include <set>

#include "vertex.hpp"
#include "util.hpp"

using namespace std;

/* Forward declarations. */
class Vertex;

class Instance {
    private:
        int _n_terminals;       // Number of given terminals.
        int _n_vertices;        // Number of vertices in the Hanan grid 
                                // generated by the terminals.

        vector<Vertex*> _V;     // References to all vertices in 
                                // lexicographical order.
        Vertex**** _hanan_grid; // References to all vertices, ordered according
                                // to the Hanan grid.
        Vertex** _terminals;    // References to all terminals.

        set<int> _x_values;     // All occuring x-coordinates of terminals.
        set<int> _y_values;     // All occuring y-coordinates of terminals.
        set<int> _z_values;     // All occuring z-coordinates of terminals.
        int _n_x_values;        // Size of _x_values
        int _n_y_values;        // Size of _y_values
        int _n_z_values;        // Size of _z_values

        /* Find all x,y,z values appearing in terminal locations 
        *  for later use in finding the Hanan grid. Save how many 
        *  different x, y, z values were found respectively. */
        Result SetXYZ(int **term_locs);

        /* Create all vertices in the Hanan grid generated by the terminals
         * and add references to them to _V. */
        Result SetVertices();

        /* Add references to all vertices to the 3D array _hanan_grid, in
         * correspondence with their (x,y,z)-position in the Hanan grid. */
        Result SetHananGrid();

        /* Add references to each vertex's neighbours in
         * the Hanan Grid. */
        Result SetNeighbours();

        /* Save references to all terminals in _terminals. */
        Result SetTerminals(int **term_locs);

        /* Find the terminal closest to the center of gravity and make
         * it so that _terminals[0] corresponds to the vertex closest to
         * the center of gravity of all terminals. */
        Result SortTerminals();

        /* Assign an ID to all vertices, based on their position in _V. */
        Result SetIds();

    public:
        /* Constructor / Destructor. */
        Instance(int n, int **term_locs);
        ~Instance();

        /* Getters / Setters. */
        const int &GetNTerminals() const;
        const int &GetNVertices() const;
        const vector<Vertex*> &GetVertices() const;
        Vertex **GetTerminals();

        /* IO-functions for testing. */
        void PrintTerminals();
        void PrintVertices();
};

#endif
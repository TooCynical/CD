/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * solver.hpp
 *
 *
 * Solver is a class that, given a valid Instance object representing
 * an instance of the 3d rectilinear steiner tree problem contains 
 * functionality to compute an optimal solution to this problem using
 * the Dijsktra-Steiner algorithm with future costs and pruning, as 
 * described by Hougardy et al. (2015).
 * The lower and upper bounds on labels are computed by a BoundComputator
 * object, which the solver will initialize based on given options (i.e.
 * Bounding Box, One-Tree or both). The solver itself, then,
 * performs the core algorithm and keeps track of the priority queue
 * used. Detailed explanation of each of Solver's members and methods
 * are found in the declarations below. 
 *
 *
 * Solver assumes, but does not check, that:
 *      The size of the vector of vertices in the given instance does not 
 *      exceed 64^3.
 *      The array of terminals in the given instance contains only references
 *      to vertices of the given instance, and its size does not exceed 
 *      the size of the constant BITSET_SIZE =< 64.     
 *      The x,y,z-values of the vertices in the given instance are all EVEN,
 *      and their values are between 0 - 2000.
 *      The vertices and terminals in the given instance are all unique,
 *      that is no two terminals have the same coordinates, and no two 
 *      vertices have the same coordinates.
 *
 *
 * Notation used:
 *      N           : The priority queue. 
 *      P           : The set of all permanently valued labels. 
 *      R           : The set of all terminals
 *      l(v,I)      : the (tentative) value of a label.
 *      LB(v, I)    : the (permanent) lower bound for a label.
 *      root        : The root terminal (always the first terminal
 *                    in the array _terminals in the given instance).
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "util.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "label.hpp"
#include "bound_computator.hpp"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <queue>
#include <iterator>
#include <cmath>
#include <unordered_map>

using namespace std;

/* Comparator for pairs of integers and labels. Simply compares 
 * the integers. */
struct LabelValuePairComp {
    bool operator()(const pair<int, Label*> &lhs, 
                    const pair<int, Label*> &rhs) const {
        return lhs.first > rhs.first;
    }
};

class Solver {
    private:
        Instance *_problem_instance;    // The underlying problem instance.
        BoundComputator *_bound_comp;   // Bound computator for this instance.
        int _global_upper_bound;        // Upper bound for this instance.

        /* Priority queue of lables added during the algorithm. This 
         * horrible line initializes a queue of pairs of integers and
         * references to labels. The integers represent the value 
         * l(v, I) + LB(v, R-I) AT THE TIME THE PAIR IS ADDED 
         * TO THE QUEUE. We need this implementation to deal with the 
         * dynamic nature of these values. */
        priority_queue<pair<int, Label*>, vector<pair<int, Label*> >,
                       LabelValuePairComp> _N;
        

        /* Set a global upper bound for the current instance by
         * computing the value of an MST on R. */
        Result SetGlobalUpperBound();

        /* Add a label to the priority queue _N and
        *  compute the lower bound for the label 
        *  if this hasn't happened before. */
        Result AddLabelToN(Label *l);

        /* Add (s, {s}) to N for each terminal s unequal to 
         * the root. */
        Result SetInitialN();

        /* Add (s, emptyset) for all vertices s with 
         * l(s, emptyset) = 0 and put these labels in P. */
        Result SetInitialLabels();

        /* Consider for the given label (v, I) all neighbours
         * (w, I) and perform the Dijkstra-step if needed. */   
        Result ConsiderNeighbours(Label *l);

        /* Perform the merge-step for the given label. */
        Result Merge(Label *v_label);

    public:
        /* Constructor / Destructor. */
        Solver(Instance *problem_instance);
        ~Solver();

        /* Attempt to solve the current instance. Return SUCCESS and
         * place solution in ret if successful, return FAIL otherwise. */
        Result SolveCurrentInstance(int &ret);
};

#endif
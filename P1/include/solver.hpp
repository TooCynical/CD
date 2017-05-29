#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "util.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "label.hpp"

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
         * l(v, I) + LowerBound(v, R-I) AT THE TIME THE PAIR IS ADDED 
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

        /* Add (s, {s}) to _N for each terminal s unequal to 
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
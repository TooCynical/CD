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

/* Comparator for pairs of values and labels. Simply compares 
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
        int _solution_value;            // Value of the solution to
                                        // given instance.
        bool _solution_found;           // Has a solution been found?
        int _global_upper_bound;        // Upper bound for this instance.

        /* Priority queue of lables added during the algorithm. This 
         * horrible line initializes a queue of pairs of integers and
         * references to labels. The integers represent the value 
         * l(v, I) + LowerBound(v, R-I) AT THE TIME THE PAIR IS ADDED 
         * TO THE QUEUE. We need this implementation to deal with the 
         * dynamic nature of these values. */
        priority_queue<pair<int, Label*>, vector<pair<int, Label*> >,
                       LabelValuePairComp> _N;
        
        BoundComputator *_lower_bound_comp;

        Result AddLabelToN(Label *l);

        Result SetGlobalUpperBound();
        Result SetInitialN();
        Result SetInitialLabels();

        Result ConsiderNeighbours(Label *l);
        Result Merge(Label *v_label);

    public:
        /* Constructor / Destructor. */
        Solver(Instance *problem_instance);
        ~Solver();

        Result SolveCurrentInstance();
        Result GetSolution(int &ret);

        void Test();
};

#endif
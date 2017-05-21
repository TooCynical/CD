#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "util.hpp"
#include "vertex.hpp"
#include "instance.hpp"
#include "label.hpp"


#include <iostream>
#include <stdlib.h>
#include <bitset>
#include <queue>
#include <iterator>
#include <cmath>


using namespace std;

class Solver {
    private:
        Instance *_problem_instance;            // The underlying problem instance.
        priority_queue<Label*, vector<Label*>,  // Priority Queue of lables 
                       LabelPointerComp > _N;   // added during the algorithm.
        int _solution_value;                    // Value of the solution to
                                                // given instance.
        bool _solution_found;                   // Has a solution been found?

        /* Hash table containing MST(I) for a terminal set I, if 
         * it has been computed already. */
        unordered_map<bitset<BITSET_SIZE>, int> _label_hash;   

        Result AddLabelToN(Label *l);

        Result SetInitialN();
        Result SetInitialLabels();

        Result ConsiderNeighbours(Label *l);
        Result Merge(Label *v_label);

    public:
        /* Constructor */
        Solver(Instance *problem_instance);
        
        Result SolveCurrentInstance();
        Result GetSolution(int &ret);
};

#endif
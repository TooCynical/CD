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


using namespace std;

class Solver {
    private:
        Instance *_problem_instance;
        priority_queue<Label*, vector<Label*>, LabelPointerComp > _N;
        int _solution_value;

        Result SetInitialN();
        Result SetInitialLabels();
        Result ConsiderNeighbours(Label *l);
        Result Merge(Label *v_label);
        Result AddLabelToN(Label *l);


    public:
        Solver(Instance *problem_instance);
        Result SolveCurrentInstance();
};

int RectDistance(Vertex *v, Vertex *w);

#endif
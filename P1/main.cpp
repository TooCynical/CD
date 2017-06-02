#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    if (MAX_TERMINALS > BITSET_SIZE) {
        cout << "Constant MAX_TERMINALS > constant BITSET_SIZE which " <<
                "is not allowed. Please modify this in util.hpp!\n";
        exit(1);
    }

    Instance *inst;
    if (ParseFile(argv[1], inst) == FAIL) {
        cout << "Main: Failed to parse file.\n";
        exit(1);
    }

    /* Make a BoundComputator. */
    BoundOptions *opts = (BoundOptions*) calloc(1, sizeof(BoundOptions));
    opts->_use_BB_lower_bound = true;
    opts-> _use_onetree_lower_bound = false;
    BoundComputator *bound_comp = new BoundComputator(inst, opts);

    Solver *S = new Solver(inst, bound_comp);

    int solution;
    if (S->SolveCurrentInstance(solution) == FAIL) {
        cout << "Main: failed to find solution.\n";
        exit(1);
    }

    if (DOUBLE_INPUT_COORDS)
        solution /= 2;

    cout << solution << "\n";

    delete inst;
    delete bound_comp;
    delete S;
}
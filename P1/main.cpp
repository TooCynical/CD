#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    if (MAX_TERMINALS > BITSET_SIZE) {
        cout << "Constant MAX_TERMINALS > constant BITSET_SIZE which " <<
                "is not allowed. Please modify this in util.hpp!\n";
        exit(1);
    }

    /* Check if an argument was given. */
    if (argc < 2) {
        cout << "Usage: ./bin/main filename [ALL / OT / BB / NONE]\n";
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
    opts->_use_onetree_lower_bound = false;

    /* Read options for lower bounds */
    if (argc > 2) {
        string bound_arg(argv[2]);
        if (bound_arg.compare("ALL") == 0) {
            opts->_use_BB_lower_bound = true;
            opts->_use_onetree_lower_bound = true;
        }
        if (bound_arg.compare("OT") == 0) {
            opts->_use_BB_lower_bound = false;
            opts->_use_onetree_lower_bound = true;
        }
        if (bound_arg.compare("BB") == 0) {
            opts-> _use_BB_lower_bound = true;
            opts->_use_onetree_lower_bound = false;
        }
        if (bound_arg.compare("NONE") == 0) {
            opts->_use_BB_lower_bound = false;
            opts-> _use_onetree_lower_bound = false;
        }
    }

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
    exit(0);
}
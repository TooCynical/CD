#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <iostream>
#include <bitset>

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

    Solver *S = new Solver(inst);
    
    int solution;
    if (S->SolveCurrentInstance(solution) == FAIL)
        exit(1);

    if (DOUBLE_INPUT_COORDS)
        solution /= 2;

    cout << solution << "\n";

    delete inst;
    delete S;
}
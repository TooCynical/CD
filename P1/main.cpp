#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <iostream>
#include <bitset>

using namespace std;

int main(int argc, char* argv[]) {
    Instance *inst;
    ParseFile(argv[1], inst);

    Solver *S = new Solver(inst);
    S->SolveCurrentInstance();
    
    int solution;
    S->GetSolution(solution);
    if (DOUBLE_INPUT_COORDS)
        solution /= 2;

    // cout << "MST: " << MST(b, inst->GetTerminals(), inst->GetNTerminals()) << "\n";
    cout << "Solution: " << solution << "\n";

    delete inst;
    delete S;
}
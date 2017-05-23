#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <iostream>
#include <bitset>

using namespace std;

int main(int argc, char* argv[]) {

    if (DOUBLE_INPUT_COORDS);
        cout << "Warning: All vertex coordinates have been doubled!\n";

    Instance *inst;
    ParseFile(argv[1], inst);

    cout << "Number of terminals: " << inst->GetNTerminals() << "\n";
    cout << "Number of vertices: " << inst->GetNVertices() << "\n";

    Solver *S = new Solver(inst);
    // S->Test();
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
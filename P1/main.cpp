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
    if (ParseFile(argv[1], inst) == FAIL)
        exit(1);


    Solver *S = new Solver(inst);
    // S->Test();
    S->SolveCurrentInstance();
    
    int solution;
    if (S->GetSolution(solution) == FAIL)
        exit(1);

    if (DOUBLE_INPUT_COORDS)
        solution /= 2;

    cout << solution << "\n";

    delete inst;
    delete S;
}
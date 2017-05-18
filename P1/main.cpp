#include "parser.hpp"
#include "util.hpp"
#include "instance.hpp"
#include "solver.hpp"

#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    Instance *inst;
    ParseFile(argv[1], inst);
    Solver *S = new Solver(inst);
    S->SolveCurrentInstance();
}
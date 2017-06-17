#include "rectangle.hpp"
#include "instance.hpp"
#include "util.hpp"
#include "parser.hpp"
#include "sequence_pair.hpp"
#include "solver.hpp"
#include "directed_acyclic_graph.hpp"

#include <iostream>

using namespace std;

int main(int argc, const char **argv) {

    const char *file_name;
    if (argc > 1)
        file_name = argv[1];
    else
        file_name = "testing/cases/small_test_1";

    Instance inst;
    if (ParseFile(file_name, inst) == FAIL) {
        cout << "Main: Failed to parse file" << endl;
        exit(1);
    }
    if (inst.verify() == FAIL) {
        cout << "Main: Failed to verify instance" << endl;
        exit(1);
    }

    Solver solver(inst);

    Floorplan *floorplan;
    if (solver.solve_instance(floorplan) == FAIL)
        cout << "Main: something went wrong solving the instance." << endl;
    else
        floorplan->print_floorplan();
    return 0;
}
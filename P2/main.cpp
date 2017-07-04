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
    else {
        cout << "Usage: " << argv[0] << " <filename> [FULL]" << endl;
        exit(0);
    }

    /* Should we print simple output or full floorplan (i.e. 
     * include rectangle dimensions)? */
    bool full_floorplan = false;
    if (argc > 2) {
        string bound_arg(argv[2]);
        if (bound_arg.compare("FULL") == 0) {
            full_floorplan = true;
        }
    }

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

    if (solver.solve_instance(floorplan, 0, 0) == FAIL) {
        cout << "Main: something went wrong solving the instance." << endl;
        exit(1);
    }
    else {
        if (floorplan->verify() == FAIL) {
            cout << "Main: solution found but not legal." << endl;
            delete floorplan;
            exit(1);
        }
        else {
            if (full_floorplan)
                floorplan->print_floorplan_with_dimensions();
            else
                floorplan->print_floorplan();
        }
        delete floorplan;
    }
    return 0;
}
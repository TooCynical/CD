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

    // inst.print_rectangles();

    // Sequence S(4);
    // S.print_sequence();

    // while(S.increment() != FAIL) {
    //     S.print_sequence();
    // }

    // SequencePair SP(3);
    // SP.print_sequence_pair();

    // vector<size_t> weights = {10, 20, 30};
    // SequencePairDAG SPDAG(3, VERTICAL, weights, SP);
    // cout << SPDAG.longest_path_length() << endl;
    
    Solver S(inst);
    S.solve_instance();
    return 0;
}
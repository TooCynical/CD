/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * solver.cpp
 *
 */

 #include "solver.hpp"
 #include "directed_acyclic_graph.hpp"

using namespace std;

Solver::Solver(const Instance &inst) : _inst(inst), 
                                       _n(inst.n_rectangles())
{
    if (inst.verify() == FAIL)
        cout << "Solver: Warning: unverified instance" << endl;
}

size_t Solver::chip_height() {
    return 0;
}

size_t Solver::chip_width() {
    return 0;
}

Result Solver::set_rectangle_widths() {
    for (size_t i = 0; i < _n; i++)
        _rectangle_widths.push_back(_inst.rectangles()[i].width());
    return SUCCESS;
}


Result Solver::set_rectangle_heights() {
    for (size_t i = 0; i < _n; i++)
        _rectangle_heights.push_back(_inst.rectangles()[i].height());
    return SUCCESS;
}

// Result Solver::set_rectangle_heights();



Result Solver::solve_instance() {
    set_rectangle_widths();
    set_rectangle_heights();

    SequencePair *_seq_pair = new SequencePair(_n);
    SequencePairDAG *_seq_pair_dag_vert = 
        new SequencePairDAG(_n,
                            VERTICAL, 
                            _rectangle_heights,
                            _seq_pair);
    SequencePairDAG *_seq_pair_dag_hori = 
        new SequencePairDAG(_n,
                            HORIZONTAL, 
                            _rectangle_widths,
                            _seq_pair);
    do {
        size_t W = _seq_pair_dag_hori->longest_path_length();
        size_t H = _seq_pair_dag_vert->longest_path_length();
        cout << W << "x" << H << " = " << W*H << endl; 
    } while (_seq_pair->increment() != FAIL);
    return SUCCESS;
}
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

    set_rectangle_widths();
    set_rectangle_heights();

    _seq_pair = new SequencePair(_n);
    
    /* Make a vertical and a horizontal graph based on the
     * the sequence pair. */
    _seq_pair_dag_vert = new SequencePairDAG(_n,
                                             VERTICAL, 
                                            _rectangle_heights,
                                            _seq_pair);
    _seq_pair_dag_hori = new SequencePairDAG(_n,
                                             HORIZONTAL, 
                                            _rectangle_widths,
                                            _seq_pair);
}

Solver::~Solver() {
    delete _seq_pair_dag_hori;
    delete _seq_pair_dag_vert;
    delete _seq_pair;
}

size_t Solver::chip_height() {
    return _seq_pair_dag_vert->longest_path_length();
}

size_t Solver::chip_width() {
    return _seq_pair_dag_hori->longest_path_length();
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

Result Solver::solve_instance() {
    
    vector<size_t> best_Y_coords;
    vector<size_t> best_X_coords;
    size_t best_area = 0;

    /* For each order of the sequence pair, get chip with and
     * height, and compare area to previous best. If an improvement
     * is found, store the X and Y coords of the rectangles. */
    do {
        size_t W = chip_width();
        size_t H = chip_height();
        if (W * H < best_area || best_area == 0) {
            best_area = W * H;
            best_X_coords = _seq_pair_dag_hori->total_weights_in_order();
            best_Y_coords = _seq_pair_dag_vert->total_weights_in_order();
        }
    } while (_seq_pair->increment() != FAIL);
    
    cout << best_area << endl;
    print_vector(best_X_coords);
    print_vector(best_Y_coords);

    return SUCCESS;
}
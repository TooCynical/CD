/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * directed_acyclic_graph.cpp
 *
 */

#include "directed_acyclic_graph.hpp"

using namespace std;

SequencePairDAG::SequencePairDAG(size_t n,
                                 Orientation orientation,
                                 vector<size_t> &vertex_weights,
                                 SequencePair *seq_pair) :
                                 _n(n),
                                 _orientation(orientation),
                                 _vertex_weights(vertex_weights),
                                 _seq_pair(seq_pair)
{}

bool SequencePairDAG::is_edge(size_t x, size_t y) {
    if (_orientation == HORIZONTAL) {
        return _seq_pair->leftof(x, y);
    }
    else
        return _seq_pair->below(x, y);
}

size_t SequencePairDAG::longest_path_length() {
    vector<size_t> topo_order = _seq_pair->neg_seq().sequence();
    vector<size_t> total_weights = _vertex_weights;

    for (size_t i = 1; i <= _n; i++){
        for (size_t j = i+1; j <= _n; j++) {
            size_t index1 = topo_order[_n-i];
            size_t index2 = topo_order[_n-j];

            if (is_edge(index2, index1))
                total_weights[index2] += total_weights[index1];
            // else
                // cout << "no edge: " << index2 << ", " << index1 << endl;
            // print_vector(total_weights);
        }
    }
    // cout << endl;
    return *max_element(total_weights.begin(), total_weights.end());
}
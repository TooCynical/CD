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
{
    _total_weights = vector<size_t >(_n, 0);
}

bool SequencePairDAG::is_edge(size_t x, size_t y) {
    if (_orientation == HORIZONTAL) {
        return _seq_pair->leftof(x, y);
    }
    else
        return _seq_pair->below(x, y);
}

Result SequencePairDAG::update_topo_order_and_weights() {
    
    /* Extract topological ordering from sequence pair. */
    switch(_orientation) {
        case HORIZONTAL:
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        case VERTICAL:
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        default:
            cout << "Warning: update_topo_order_and_weights "
                 << "orientation switch defaulted." << endl;
            return FAIL;
    }

    /* Reset total weights to (initial) vertex_weights 
     * sorted according to topological order */
    for (size_t i = 0; i < _n; i++)
        _total_weights[i] = _vertex_weights[_topo_order[i]];

    /* Apply the longest path algorithm for DAGs. */
    for (size_t i = 1; i <= _n; i++){
        for (size_t j = i+1; j <= _n; j++) {
            size_t index1 = _topo_order[_n-i];
            size_t index2 = _topo_order[_n-j];

            if (is_edge(index2, index1))
                _total_weights[index2] += _total_weights[index1];
        }
    }
    return SUCCESS;
}

vector<size_t> SequencePairDAG::total_weights_in_order() {
    update_topo_order_and_weights();
    vector<size_t> ret(_n, 0);
    for (size_t i = 0; i < _n; i++) {
        ret[_topo_order[i]] = _total_weights[i];
    }
    return ret;
}

size_t SequencePairDAG::longest_path_length() {
    update_topo_order_and_weights();
    return *max_element(_total_weights.begin(), _total_weights.end());
}
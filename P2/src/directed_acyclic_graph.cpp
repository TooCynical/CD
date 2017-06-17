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
    update();
}

bool SequencePairDAG::is_edge(size_t x, size_t y) {
    switch(_orientation) {
        case HORIZONTAL:
            return _seq_pair->leftof(x, y);
        case VERTICAL:
            return _seq_pair->below(x, y);
        default:
            cout << "Error SequencePairDAG::is_edge defaulted." << endl;
            exit(1);
    }
    return 0;
}

Result SequencePairDAG::update_topo_order() {
    switch(_orientation) {
        /* As x 'leftof' y implies x < y in the negative sequence,
         * the negative sequene is a topological order for the 
         * HORIZONTAL DAG. */
        case HORIZONTAL: 
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        /* As x 'below' y implies x < y in the negative sequence,
         * the negative sequene is a topological order for the 
         * VERTICAL DAG. */    
        case VERTICAL:
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        /* We should never default on this switch. */
        default:
            cout << "Warning: update_topo_order_and_weights "
                 << "orientation switch defaulted." << endl;
            return FAIL;
    }
    return SUCCESS;
}

Result SequencePairDAG::update_total_weights() {
    /* Reset total weights to (initial) vertex_weights 
     * sorted according to topological order */
    for (size_t i = 0; i < _n; i++)
        _total_weights[i] = _vertex_weights[_topo_order[i]];

    /* Apply the topological longest path algorithm for DAGs: */
    /* Loop over vertices in topological order. */
    for (size_t i = 0; i < _n; i++){
        /* For each (outgoing) neighhbour of current vertex,
         * update longest path length if needed. */
        for (size_t j = i+1; j < _n; j++) {
            if (is_edge(_topo_order[i], _topo_order[j])) {
                if (_total_weights[j] < _total_weights[i] + 
                                        _vertex_weights[_topo_order[j]])
                {
                _total_weights[j] = _total_weights[i] + 
                                    _vertex_weights[_topo_order[j]];
                }
            }
        }
    }
    return SUCCESS;
}

Result SequencePairDAG::update() {
    if (update_topo_order() == FAIL)
        return FAIL;
    if (update_total_weights() == FAIL)
        return FAIL;
    return SUCCESS;
}

vector<size_t> SequencePairDAG::total_weights_in_order() {
    update();
    vector<size_t> ret(_n, 0);
    for (size_t i = 0; i < _n; i++) {
        ret[_topo_order[i]] = _total_weights[i];
    }
    return ret;
}

size_t SequencePairDAG::longest_path_length() {
    update();
    return *max_element(_total_weights.begin(), _total_weights.end());
}
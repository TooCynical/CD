/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * directed_acyclic_graph.cpp
 *
 * Implementation of functionality described in directed_acyclic_graph.hpp
 */

#include "directed_acyclic_graph.hpp"

using namespace std;

SequencePairDAG::SequencePairDAG(unsigned n,
                                 Orientation orientation,
                                 vector<unsigned> &vertex_weights,
                                 SequencePair *seq_pair) :
                                 _n(n),
                                 _orientation(orientation),
                                 _vertex_weights(vertex_weights),
                                 _seq_pair(seq_pair)
{
    _total_weights = vector<unsigned >(_n, 0);
    update();
}

bool SequencePairDAG::is_edge(unsigned x, unsigned y) {
    switch(_orientation) {
        case HORIZONTAL_LR:
            return _seq_pair->leftof(x, y);
        case VERTICAL_DU:
            return _seq_pair->below(x, y);
        case HORIZONTAL_RL:
            return _seq_pair->rightof(x, y);
        case VERTICAL_UD:
            return _seq_pair->above(x, y);
        default:
            cout << "Error: SequencePairDAG::is_edge defaulted." << endl;
            exit(1);
    }
    return 0;
}

Result SequencePairDAG::update_topo_order() {
    switch(_orientation) {
        /* As x 'leftof' y implies x < y in the negative sequence,
         * the negative sequene is a topological order for the 
         * HORIZONTAL_LR DAG. */
        case HORIZONTAL_LR: 
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        /* As x 'below' y implies x < y in the negative sequence,
         * the negative sequene is a topological order for the 
         * VERTICAL_DU DAG. */    
        case VERTICAL_DU:
            _topo_order = _seq_pair->neg_seq().sequence();
            break;
        /* As x 'rightof' y implies x > y in the negative sequence,
         * the reverse of the negative sequence is a topological order 
         * for the HORIZONTAL_RL DAG. */
        case HORIZONTAL_RL: 
            _topo_order = _seq_pair->neg_seq().sequence();
            reverse(_topo_order.begin(), _topo_order.end());
            break;
        /* As x 'above' y implies x < y in the positive sequence,
         * the positive sequence is a topological order for the 
         * VERTICAL_UD DAG. */    
        case VERTICAL_UD:
            _topo_order = _seq_pair->pos_seq().sequence();
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
    /* Reset total weights to (initial) vertex weights 
     * sorted according to topological order. */
    for (unsigned i = 0; i < _n; i++)
        _total_weights[i] = _vertex_weights[_topo_order[i]];

    /* Apply the topological longest path algorithm for DAGs: */
    /* Loop over vertices in topological order. */
    for (unsigned i = 0; i < _n; i++){
        /* For each (outgoing) neighbour of current vertex,
         * update longest path length if needed. */
        for (unsigned j = i+1; j < _n; j++) {
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

vector<unsigned> SequencePairDAG::total_weights_in_order() {
    #ifndef OPTIMIZED_BUILD
    /* In the main routine, it is not needed to do this update,
     * but it is unsafe otherwise not to update here. */
    if (update() == FAIL) {
        cout << "SPDAG: Error: failed to update" << endl;
        exit(1);
    }
    #endif
    
    vector<unsigned> ret(_n, 0);
    for (unsigned i = 0; i < _n; i++) {
        ret[_topo_order[i]] = _total_weights[i];
    }
    return ret;
}

unsigned SequencePairDAG::longest_path_length() {
    if (update() == FAIL) {
        cout << "SPDAG: Error: failed to update" << endl;
        exit(1);
    }

    return *max_element(_total_weights.begin(), _total_weights.end());
}
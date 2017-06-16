/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * directed_acyclic_graph.hpp
 *
 */

#ifndef DIRECTEDACYCLICGRAPH_H
#define DIRECTEDACYCLICGRAPH_H

#include <vector>
#include <algorithm>

#include "util.hpp"
#include "sequence_pair.hpp"

typedef enum {VERTICAL, HORIZONTAL} Orientation;

class SequencePairDAG {
private:
    size_t _n;
    Orientation _orientation;
    const std::vector<size_t> &_vertex_weights; // Weights of vertices, in order.
    SequencePair *_seq_pair;

    /* Note that the vectors below should be computed (manually) each time 
     * the underlying sequence pair is changed. If not, they do not reflect
     * the correct values for the current order of the sequence pair, but rather
     * for the order the sequence pair had when they were last updated. */
    std::vector<size_t> _topo_order;    // Topological order of the DAG.
    std::vector<size_t> _total_weights; // The i-th entry of the vector contains the
                                        // length of a longest path in the DAG ending
                                        // in the i-th vertex according to the 
                                        //topological ordering.

    /* Update the topological ordering and the total weights to reflect the
     * situation in the current ordering of the underlyin sequence pair. This
     * function is not split by design, as updating one but not the other is 
     * dangerous. */
    Result update_topo_order_and_weights();

    /* Return whether there is an edge between vertex x and y in the DAG,
     * based on the 'below', 'leftof', 'rightof' or 'above' relation in the
     * underlying sequence pair, depending on the orientation of the DAG. */
    bool is_edge(size_t x, size_t y);

public:
    SequencePairDAG(size_t n,
                    Orientation orientation,
                    std::vector<size_t> &vertex_weights,
                    SequencePair *seq_pair);
    
    
    /* Return the total weights of the vertices of the DAG, for the current
     * state of the underlying sequence pair. (as opposed to the ordering
     * the DAG keeps, which is topological). */
    std::vector<size_t> total_weights_in_order();

    /* Return the length of a longest path in the DAG, for the current state
     * of the underlying sequence pair. */
    size_t longest_path_length();


};

#endif
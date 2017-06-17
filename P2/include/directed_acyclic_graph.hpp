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

typedef enum {VERTICAL_DU, HORIZONTAL_LR, VERTICAL_UD, HORIZONTAL_RL} Orientation;

class SequencePairDAG {
private:
    size_t _n;                                  // Number of vertices
    Orientation _orientation;                   // Orientation of graph.
    const std::vector<size_t> &_vertex_weights; // Vertex weights in normal order.
    SequencePair *_seq_pair;                    // Underlying sequence pair of DAG.

    std::vector<size_t> _topo_order;    // Topological order of the DAG.
    std::vector<size_t> _total_weights; // Longest path lengths in the DAG in 
                                        // topological order.

    /* Update topological order to reflect the current situation in the 
     * underlying sequence pair. */
    Result update_topo_order();

    /* Update total weights to reflect the current situation in the 
     * underlying sequence pair. This function assumes that the topological
     * order is already updated. */
    Result update_total_weights();

    /* Update both topological order and total weights. */
    Result update();

    /* Return whether there is an edge between vertex x and y in the DAG,
     * based on the 'below', 'leftof', 'rightof' or 'above' relation in the
     * underlying sequence pair, depending on the orientation of the DAG. */
    bool is_edge(size_t x, size_t y);

public:
    SequencePairDAG(size_t n,
                    Orientation orientation,
                    std::vector<size_t> &vertex_weights,
                    SequencePair *seq_pair);
    
    /* Return the total weights of the vertices of the DAG (as opposed to 
     * the ordering the DAG keeps, which is topological). */
    std::vector<size_t> total_weights_in_order();

    /* Return the length of a longest path in the DAG. */
    size_t longest_path_length();


};

#endif
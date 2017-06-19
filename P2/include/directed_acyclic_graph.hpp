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

/* SequencePairDAG is a class that, given a SequencePair of size n, an orientation,
 * and a vector of n vertex weights represents the Directed Acyclic Graph generated
 * by the sequence pair for the given orientation and vertex weights. The orientation
 * here determines in what way the sequence pair determines which edges are in 
 * the DAG:
 *      - VERTICAL_DU:      (x, y) in E(DAG) iff x 'below' y in sequence pair.
 *      - HORIZONTAL_LR:    (x, y) in E(DAG) iff x 'leftof' y in sequence pair.
 *      - VERTICAL_UD:      (x, y) in E(DAG) iff x 'above' y in sequence pair.
 *      - HORIZONTAL_RL:    (x, y) in E(DAG) iff x 'rightof' y in sequence pair.
 *
 * The SequencePairDAG is able to compute the length of a longest path starting
 * in each of its vertices, using the topological order algorithm for longest paths
 * in DAGs. The topological order here can be extracted from one of the sequences
 * in the sequence pair (details are in the implementation of update_topo_order()).
 *
 * Of course, as the order on the sequence pair changes, the DAG will no longer be
 * up to date, so whenever path lengths are requested the DAG will update all of 
 * its data (which takes O(n^2) time).
 *
 * Detailed information on all members can be found below.
 */
class SequencePairDAG {
private:
    unsigned long long _n;                                  // Number of vertices
    const Orientation _orientation;             // Orientation of graph.
    const std::vector<unsigned long long> &_vertex_weights; // Vertex weights in normal order.
    SequencePair *_seq_pair;                    // Underlying sequence pair of DAG.

    std::vector<unsigned long long> _topo_order;    // Topological order of the DAG.
    std::vector<unsigned long long> _total_weights; // Longest path lengths in the DAG in 
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
    bool is_edge(unsigned long long x, unsigned long long y);

public:
    SequencePairDAG(unsigned long long n,
                    Orientation orientation,
                    std::vector<unsigned long long> &vertex_weights,
                    SequencePair *seq_pair);
    
    /* Return the total weights of the vertices of the DAG (as opposed to 
     * the ordering the DAG keeps, which is topological). */
    std::vector<unsigned long long> total_weights_in_order();

    /* Return the length of a longest path in the DAG. */
    unsigned long long longest_path_length();
};

#endif
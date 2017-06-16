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

    bool is_edge(size_t x, size_t y);
public:
    SequencePairDAG(size_t n,
                    Orientation orientation,
                    std::vector<size_t> &vertex_weights,
                    SequencePair *seq_pair);
    size_t longest_path_length();
};

#endif
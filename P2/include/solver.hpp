/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * solver.hpp
 *
 */

#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <stdlib.h>
#include <iostream>

#include "util.hpp"
#include "instance.hpp"
#include "sequence_pair.hpp"
#include "directed_acyclic_graph.hpp"


typedef enum {UPPER_RIGHT, UPPER_LEFT, LOWER_RIGHT, LOWER_LEFT} Origin; 

class Floorplan {
private:
    const Instance &_inst;  // Underlying instance.
    size_t _n;              // Number of rectangles in underlying instance.
    
    size_t _width;                  // Width taken by the floorplan.
    size_t _height;                 // Height taken by the floorplan.
    size_t _area;                   // Area taken by the floorplan.
    std::vector<size_t> _x_coords;  // X-coords of rectangles (origin lower-left).
    std::vector<size_t> _y_coords;  // Y-coords of rectangles (origin lower-left).

    /* Convert coords given in constructor (with orientation) to
     * lower-left orientation. */
    Result set_coords(std::vector<size_t> x_coords,
                        std::vector<size_t> y_coords,
                        Origin ori);
public:
    Floorplan(const Instance &inst,
              size_t width,
              size_t height,
              std::vector<size_t> x_coords,
              std::vector<size_t> y_coords,
              Origin ori);

    Result print_floorplan();
};


class Solver {
private:
    const Instance &_inst;          // Underlying instance.
    size_t _n;                      // Number of rectangles in underlying instance.

    std::vector<size_t> _rectangle_widths;
    std::vector<size_t> _rectangle_heights;

    SequencePair *_seq_pair;
    SequencePairDAG *_seq_pair_dag_hori;
    SequencePairDAG *_seq_pair_dag_vert;

    size_t chip_height();
    size_t chip_width();

    Result set_rectangle_widths();
    Result set_rectangle_heights();

public:
    Solver(const Instance &inst);
    ~Solver();

    Result solve_instance(Floorplan *&ret);
};

#endif
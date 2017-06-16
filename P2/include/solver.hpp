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

class Solver {
private:
    const Instance &_inst;          // Underlying instance.
    size_t _n;                      // Number of rectangles in underlying instance.

    std::vector<size_t> _rectangle_widths;
    std::vector<size_t> _rectangle_heights;

    size_t chip_height();
    size_t chip_width();

    Result set_rectangle_widths();
    Result set_rectangle_heights();

public:
    Solver(const Instance &inst);

    Result solve_instance();   // TODO: make return floorplan.
};

#endif
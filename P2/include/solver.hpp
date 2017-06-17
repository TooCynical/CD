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
#include <algorithm> 

#include "util.hpp"
#include "instance.hpp"
#include "sequence_pair.hpp"
#include "directed_acyclic_graph.hpp"


typedef enum {UPPER_RIGHT, UPPER_LEFT, LOWER_RIGHT, LOWER_LEFT} Origin; 

/* Floorplan is a class that, given an instance of the floorplanning problem 
 * and two vectors containing x_coordinates and y_coordinates for each of the 
 * rectangles in this instance represents a placement for these rectangles.
 * Floorplan is able to:
 *      - Accept coordinates for each origin (upper/lower left/right) and
 *        convert them to lower left coordinates.
 *      - Output the floorplan in multiple formats, as described below.
 *      - Verify that the given coordinates represent a valid floorplan.
 *
 * Detailed information on all members can be found below. */
class Floorplan {
private:
    const Instance &_inst;  // Underlying instance.
    size_t _n;              // Number of rectangles in underlying instance.
    
    size_t _width;                  // Width taken by the floorplan.
    size_t _height;                 // Height taken by the floorplan.
    size_t _area;                   // Area taken by the floorplan.
    std::vector<size_t> _x_coords;  // X-coords of rectangles (origin lower-left).
    std::vector<size_t> _y_coords;  // Y-coords of rectangles (origin lower-left).

    /* Convert coords given in constructor (with origin) to
     * lower-left origin. */
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

    /* Check that the given floorplan is valid (i.e. no overlap). */
    Result verify();

    /* Print the floorplan as per instructions in the exercise. */
    Result print_floorplan();

    /* Print the floorplan, but also add dimensions of each rectangle for
     * use in the drawer (testing/drawer.py). */
    Result print_floorplan_with_dimensions();
};


/* Solver is a class that, given a valid instance of the floorplanning problem,
 * manages functionality to compute on optimum placement for this instance,
 * represented by a Floorplan object.
 * Solver basically acts as a driver for the sequence-pair algorithm. Actual 
 * computation is performed within the SequencePair and SequencePairDAG classes.
 *
 * Solver assumes but does not check that the provided instance object passes
 * its own verification function. 
 *
 * Detailed information on all members can be found below. */
class Solver {
private:
    const Instance &_inst;          // Underlying instance.
    size_t _n;                      // Number of rectangles in underlying instance.

    std::vector<size_t> _rectangle_widths;  // Widths of rectangles in instance.
    std::vector<size_t> _rectangle_heights; // Heights of rectangles in instance.

    SequencePair *_seq_pair;                // Sequence pair of size n
    SequencePairDAG *_seq_pair_dag_hori;    // The horizontal contraint graph
                                            // based on _seq_pair.
    SequencePairDAG *_seq_pair_dag_vert;    // The vertical contraint graph
                                            // based on _seq_pair.

    /* Compute chip width/height resulting from current order of _seq_pair by
     * computing the length of a longest path in the horizontal/vertical 
     * constraint graph. */
    size_t chip_height();
    size_t chip_width();

    /* Compute a lower bound for the chip area/width/height required 
     * in a floorplan for the instance. */
    size_t get_area_lower_bound();
    size_t get_width_lower_bound();
    size_t get_height_lower_bound();

    /* Fetch rectangle widths/heights from the instance and store them
     * in vectors. */
    Result set_rectangle_widths();
    Result set_rectangle_heights();

public:
    Solver(const Instance &inst);
    ~Solver();

    /* Solve the current instance using the sequence pair method. */
    Result solve_instance(Floorplan *&ret);
};

#endif
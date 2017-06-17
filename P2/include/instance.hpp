/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * instance.hpp
 */

#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>

#include "rectangle.hpp"

class IdHelper {
private:
    size_t _current_id = 0;
public:
    Result get_id(size_t &ret);
};

/* Instance is a class that, given a nonnegative integer n, and a nx2 array 
 * of positive integers, represents the set of Rectangles with widths and 
 * heights given by this array. The Instane will create all Rectangles and 
 * provide them with a unique ID, which corresponds to their position in the 
 * array. The Instance will also verify that all rectangles were created 
 * succesfully. */
class Instance {
private:
    std::vector<Rectangle> _rectangles;
    size_t _n_rectangles;
    bool _rectangles_set = false;

    IdHelper _id_gen;

    Result add_rectangle(unsigned int width, unsigned int height);

public:
    Result set_rectangles(size_t n_rectangles, unsigned int **rectangle_dims);

    Result verify() const;

    const std::vector<Rectangle> &rectangles() const;
    size_t n_rectangles() const;

    /* IO-functions for testing. */
    Result print_rectangles() const;
};

#endif
/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * instance.hpp
 *
 */
#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>

#include "rectangle.hpp"

class IdHelper {
private:
    size_t _current_id = 0;
public:
    size_t get_id();
};


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
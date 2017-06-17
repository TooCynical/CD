/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * rectangle.hpp
 *
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "util.hpp"
#include <iostream>

class Rectangle {
private:
    const unsigned int _width;
    const unsigned int _height;
    const size_t _id;

public:
    Rectangle(unsigned int width, unsigned int height, size_t id); 
    const unsigned int &width() const;
    const unsigned int &height() const;
    const size_t &id() const;

    /* IO-functions for testing. */
    Result print_rectangle() const;
};

/* Return whether the given rectangles intersect when placed at
 * the given coordinates. */
bool intersect(const Rectangle &a, size_t x_a, size_t y_a,
               const Rectangle &b, size_t x_b, size_t y_b);

#endif
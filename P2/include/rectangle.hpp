/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * rectangle.hpp
 */

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "util.hpp"
#include <iostream>

/* Rectangle is a class that, given three nonnegative integers represents
 * the rectangle of width and height given by the first two integers.
 * Furthermore, it keeps an ID, given by the third integer used for verification.
 * Essentially this is a fancy wrapper for two unsigned ints. */
class Rectangle {
private:
    const unsigned _width;
    const unsigned _height;
    const unsigned _id;

public:
    Rectangle(unsigned width, unsigned height, unsigned id); 
    const unsigned &width() const;
    const unsigned &height() const;
    const unsigned &id() const;

    /* IO-functions for testing. */
    Result print_rectangle() const;
};

/* Return whether the given rectangles intersect when placed at
 * the given coordinates. */
bool intersect(const Rectangle &a, unsigned x_a, unsigned y_a,
               const Rectangle &b, unsigned x_b, unsigned y_b);

#endif
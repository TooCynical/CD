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
 * Essentially this is a fancy wrapper for two uint64_ts. */
class Rectangle {
private:
    const uint64_t _width;
    const uint64_t _height;
    const size_t _id;

public:
    Rectangle(uint64_t width, uint64_t height, size_t id); 
    const uint64_t &width() const;
    const uint64_t &height() const;
    const size_t &id() const;

    /* IO-functions for testing. */
    Result print_rectangle() const;
};

/* Return whether the given rectangles intersect when placed at
 * the given coordinates. */
bool intersect(const Rectangle &a, uint64_t x_a, uint64_t y_a,
               const Rectangle &b, uint64_t x_b, uint64_t y_b);

#endif
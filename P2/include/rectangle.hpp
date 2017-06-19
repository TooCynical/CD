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
 * Essentially this is a fancy wrapper for two unsigned long longs. */
class Rectangle {
private:
    const unsigned long long _width;
    const unsigned long long _height;
    const unsigned long long _id;

public:
    Rectangle(unsigned long long width, unsigned long long height, unsigned long long id); 
    const unsigned long long &width() const;
    const unsigned long long &height() const;
    const unsigned long long &id() const;

    /* IO-functions for testing. */
    Result print_rectangle() const;
};

/* Return whether the given rectangles intersect when placed at
 * the given coordinates. */
bool intersect(const Rectangle &a, unsigned long long x_a, unsigned long long y_a,
               const Rectangle &b, unsigned long long x_b, unsigned long long y_b);

#endif
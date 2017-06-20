/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * rectangle.cpp
 *
 * Implementation of functionality described in rectangle.hpp. 
 */

#include "rectangle.hpp"

using namespace std;

Rectangle::Rectangle(uint64_t width, uint64_t height, uint64_t id) : 
                    _width(width), _height(height), _id(id)
{}

Result Rectangle::print_rectangle() const {
    cout << "Rectangle " << _id << " : ("
         << _width << ", " << _height << ")\n";
    return SUCCESS;
}

const uint64_t &Rectangle::width() const { return _width; }
const uint64_t &Rectangle::height() const { return _height; }
const uint64_t &Rectangle::id() const { return _id; }

bool intersect(const Rectangle &a, uint64_t x_a, uint64_t y_a,
               const Rectangle &b, uint64_t x_b, uint64_t y_b)
{
    uint64_t x_max_a = x_a + a.width();
    uint64_t x_min_a = x_a;
    uint64_t y_max_a = y_a + a.height();
    uint64_t y_min_a = y_a;

    uint64_t x_max_b = x_b + b.width();
    uint64_t x_min_b = x_b;
    uint64_t y_max_b = y_b + b.height();
    uint64_t y_min_b = y_b;

    /* Horrible logic... */
    return ((between_right(x_min_b, x_min_a, x_max_b) || 
             between_left(x_min_b, x_max_a, x_max_b)) &&
            (between_right(y_min_b, y_min_a, y_max_b) || 
             between_left(y_min_b, y_max_a, y_max_b)));
}
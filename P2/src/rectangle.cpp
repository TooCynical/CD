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

Rectangle::Rectangle(unsigned width, unsigned height, unsigned id) : 
                    _width(width), _height(height), _id(id)
{}

Result Rectangle::print_rectangle() const {
    cout << "Rectangle " << _id << " : ("
         << _width << ", " << _height << ")\n";
    return SUCCESS;
}

const unsigned &Rectangle::width() const { return _width; }
const unsigned &Rectangle::height() const { return _height; }
const unsigned &Rectangle::id() const { return _id; }

bool intersect(const Rectangle &a, unsigned x_a, unsigned y_a,
               const Rectangle &b, unsigned x_b, unsigned y_b)
{
    unsigned x_max_a = x_a + a.width();
    unsigned x_min_a = x_a;
    unsigned y_max_a = y_a + a.height();
    unsigned y_min_a = y_a;

    unsigned x_max_b = x_b + b.width();
    unsigned x_min_b = x_b;
    unsigned y_max_b = y_b + b.height();
    unsigned y_min_b = y_b;

    /* Horrible logic... */
    return ((between_right(x_min_b, x_min_a, x_max_b) || 
             between_left(x_min_b, x_max_a, x_max_b)) &&
            (between_right(y_min_b, y_min_a, y_max_b) || 
             between_left(y_min_b, y_max_a, y_max_b)));
}
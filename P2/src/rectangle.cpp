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

Rectangle::Rectangle(unsigned int width, unsigned int height, size_t id) : 
                    _width(width), _height(height), _id(id)
{}

Result Rectangle::print_rectangle() const {
    cout << "Rectangle " << _id << " : ("
         << _width << ", " << _height << ")\n";
    return SUCCESS;
}

const unsigned int &Rectangle::width() const { return _width; }
const unsigned int &Rectangle::height() const { return _height; }
const size_t &Rectangle::id() const { return _id; }

bool intersect(const Rectangle &a, size_t x_a, size_t y_a,
               const Rectangle &b, size_t x_b, size_t y_b)
{
    size_t x_max_a = x_a + a.width();
    size_t x_min_a = x_a;
    size_t y_max_a = y_a + a.height();
    size_t y_min_a = y_a;

    size_t x_max_b = x_b + b.width();
    size_t x_min_b = x_b;
    size_t y_max_b = y_b + b.height();
    size_t y_min_b = y_b;

    /* Horrible logic... */
    return ((between_right(x_min_b, x_min_a, x_max_b) || 
             between_left(x_min_b, x_max_a, x_max_b)) &&
            (between_right(y_min_b, y_min_a, y_max_b) || 
             between_left(y_min_b, y_max_a, y_max_b)));
}
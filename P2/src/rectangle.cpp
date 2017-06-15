/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * rectangle.cpp
 *
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

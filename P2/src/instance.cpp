/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * instance.cpp
 *
 * Implementation of functionality described in instance.hpp.
 */

#include "instance.hpp"

using namespace std;

Result Instance::set_rectangles(uint64_t n_rectangles, 
                                uint64_t **rectangle_dims)
{
    if (_rectangles_set)
        return FAIL;

    _n_rectangles = n_rectangles;
    Result res;
    for (uint64_t i = 0; i < n_rectangles; i++) {
        res = add_rectangle(rectangle_dims[i][0], rectangle_dims[i][1]);
        if (res == FAIL)
            return FAIL;
    }

    _rectangles_set = true;
    return SUCCESS;
}

Result Instance::add_rectangle(uint64_t width, uint64_t height) {
    /* First get an id for the rectangle if possible. */
    uint64_t id;
    if (_id_gen.get_id(id) == FAIL)
        return FAIL;

    _rectangles.push_back(Rectangle(width, height, id));
    return SUCCESS;
}

Result Instance::verify() const {
    if (!_rectangles_set)
        return FAIL;

    if (_n_rectangles != _rectangles.size())
        return FAIL;
 
    /* Check that the dimensions of each rectangle are in bounds,
     * and that the id of each rectangle corresponds to their location
     * in the array, thus automatically checking that ids are unique. */
    for (uint64_t i = 0; i < _n_rectangles; i++) {
        if (_rectangles[i].height() > MAX_RECT_DIM ||
            _rectangles[i].width() > MAX_RECT_DIM  ||
            _rectangles[i].id() != i)
        {
            return FAIL;
        }
    }
    return SUCCESS;
}

const vector<Rectangle> &Instance::rectangles() const {
    return _rectangles;
}

uint64_t Instance::n_rectangles() const { return _n_rectangles; }

Result Instance::print_rectangles() const {
    if (!_rectangles_set) {
        cout << "Instance: Attempting to print unset rectangles" << endl;
        exit(1);
    }

    for (uint64_t i = 0; i < _n_rectangles; i++) {
        _rectangles[i].print_rectangle();
    }
    return SUCCESS;
}

Result IdHelper::get_id(uint64_t &ret) { 
    if (_current_id == SIZE_MAX)
        return FAIL;

    ret = _current_id;
    _current_id++;
    return SUCCESS;
}
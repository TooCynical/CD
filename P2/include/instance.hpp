/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * instance.hpp
 */

#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>

#include "rectangle.hpp"

class IdHelper {
private:
    size_t _current_id = 0;
public:
    Result get_id(size_t &ret);
};

/* Instance is a class that, given a nonnegative integer n, and a nx2 array 
 * of positive integers, represents the set of Rectangles with widths and 
 * heights given by this array. The Instane will create all Rectangles and 
 * provide them with a unique ID, which corresponds to their position in the 
 * array. The Instance will also verify that all rectangles were created 
 * succesfully. */
class Instance {
private:
    std::vector<Rectangle> _rectangles;     // Rectangles that make up the instance.
    size_t _n_rectangles;                   // Number of rectangles in instance.
    bool _rectangles_set = false;           // Have the rectangles been set yet?

    IdHelper _id_gen;                       // Object that generates IDs.

    /* Add a new Rectangle of given width and height to the instance. */
    Result add_rectangle(unsigned int width, unsigned int height);

public:
    /* Set Rectangles for this instances given an nx2 array: nx(width height). */
    Result set_rectangles(size_t n_rectangles, unsigned int **rectangle_dims);

    /* Verify the instance. */
    Result verify() const;

    /* Accessors. */
    const std::vector<Rectangle> &rectangles() const;
    size_t n_rectangles() const;

    /* IO-functions for testing. */
    Result print_rectangles() const;
};

#endif
#ifndef LOWERBOUND_HPP
#define LOWERBOUND_HPP

    #include "util.hpp"
    #include "vertex.hpp"
    #include "instance.hpp"
    #include "label.hpp"
    #include <stdlib.h>
    
    /* Forward declaration */
    class Label;
    class Vertex;

    int BBLowerBound(Label *l, Vertex **terminals, int n);

#endif
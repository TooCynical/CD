#include "lower_bound.hpp"

using namespace std;

/* Compute the bounding box lower bound for a 
 * a label (i.e. the bounding box lower bound for
 * a Steiner Tree on its complement) given the 
 * terminal set used and the number of total terminals */ 
int BBLowerBound(Label *l, Vertex **terminals, int n) {
    int x_max, x_min;
    int y_max, y_min;
    int z_max, z_min;
    
    /* Set initial values based on vertex in label */
    x_max = x_min = l->GetVertex()->getX();
    y_max = y_min = l->GetVertex()->getY();
    z_max = z_min = l->GetVertex()->getZ();

    /* Loop over all terminals in the complement of the labels
     * terminal set and update values accordingly */
    for (int i = 0; i < n; i ++) {
        if (!l->GetBitset()[0].test(i)) {
            if (terminals[i]->getX() < x_min)
                x_min = terminals[i]->getX();
            if (terminals[i]->getX() > x_max)
                x_max = terminals[i]->getX();
            if (terminals[i]->getY() < y_min)
                y_min = terminals[i]->getY();
            if (terminals[i]->getY() > y_max)
                y_max = terminals[i]->getY();
            if (terminals[i]->getZ() < z_min)
                z_min = terminals[i]->getZ();
            if (terminals[i]->getZ() > z_max)
                z_max = terminals[i]->getZ();
        }
    }
    return 0;
    return (x_max - x_min) + (y_max - y_min) + (z_max - z_min);
}

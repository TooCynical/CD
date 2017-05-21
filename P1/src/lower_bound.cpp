#include "lower_bound.hpp"

using namespace std;

/* Compute the bounding box lower bound for a 
 * a label (i.e. the bounding box lower bound for
 * a Steiner Tree on its complement) given the 
 * terminal set used and the number of total terminals. */ 
int BBLowerBound(Label *l, Vertex **terminals, int n) {
    /* If root terminal in the terminal set of the label 
     * return 0 */
    if (l->GetBitset()[0].test(0))
        return 0;

    int x_max, x_min;
    int y_max, y_min;
    int z_max, z_min;
    
    /* Set initial values based on vertex in label */
    x_max = x_min = l->GetVertex()->GetX();
    y_max = y_min = l->GetVertex()->GetY();
    z_max = z_min = l->GetVertex()->GetZ();

    /* Loop over all terminals in the complement of the labels
     * terminal set and update values accordingly */
    for (int i = 0; i < n; i ++) {
        if (!l->GetBitset()[0].test(i)) {
            // terminals[i]->Print();
            if (terminals[i]->GetX() < x_min)
                x_min = terminals[i]->GetX();
            if (terminals[i]->GetX() > x_max)
                x_max = terminals[i]->GetX();
            if (terminals[i]->GetY() < y_min)
                y_min = terminals[i]->GetY();
            if (terminals[i]->GetY() > y_max)
                y_max = terminals[i]->GetY();
            if (terminals[i]->GetZ() < z_min)
                z_min = terminals[i]->GetZ();
            if (terminals[i]->GetZ() > z_max)
                z_max = terminals[i]->GetZ();
        }
    }
    return (x_max - x_min) + (y_max - y_min) + (z_max - z_min);
}

/* Compute the length of a minimum spanning tree 
 * on the given subset of terminals using a O(n^2)-
 * implementation of Prim's Algorithm, where n is the 
 * total number of terminals */
 int MST(bitset<BITSET_SIZE> I, Vertex **terminals, int n) {
    /* First make new references to the relevant terminals */
    int n_rel_terminals = I.count();
    
    /* An MST on <= 1 vertex has length 0. */
    if (n_rel_terminals <= 1)
        return 0;
    
    Vertex **rel_terminals = (Vertex**) calloc(n_rel_terminals,
                                                sizeof(Vertex*));
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (I.test(i)) {
            rel_terminals[count] = terminals[i];
            count ++;
        }
    }

    /* Array containing cheapest cost of a connection to each vertex. 
     * Initially set all values to infinity except for costs[0]. */
    int *costs = (int*) calloc(n_rel_terminals, sizeof(int));
    for(int i = 1; i < n_rel_terminals; i++) {
        costs[i] = INT_MAX;
    }

    /* Array indicating which terminals are already in the tree,
     * initially add the first one. */
    bool *in_tree = (bool*) calloc(n_rel_terminals, sizeof(int));
    
    int min = INT_MAX;
    int min_index = 0;
    int tree_cost = 0;

    for (int j = 0; j < n_rel_terminals; j++) {
        min = INT_MAX;
        /* Find terminal with minimum cost and add it to tree. */
        for (int i = 0; i < n_rel_terminals; i++) {
            if (costs[i] < min && !in_tree[i]) {
                min = costs[i];
                min_index = i;
            }
        }

        /* Update terminal costs and total tree cost. */
        tree_cost += costs[min_index];
        in_tree[min_index] = true;
        for (int i = 0; i < n_rel_terminals; i++) {
            if (RectDistance(rel_terminals[i], rel_terminals[min_index]) < 
                costs[i] && i != min_index) {
                costs[i] = RectDistance(rel_terminals[i], 
                                        rel_terminals[min_index]);
            }
        }
    }

    free(rel_terminals);
    free(costs);
    free(in_tree);

    return tree_cost;
 }

/* Compute the MST Lower Bound for the complement of the 
 * terminal set of the given label */
int MSTLowerBound(Label *l, Vertex **terminals, int n) {
    /* If root terminal in the terminal set of the label 
     * return 0. */
    if (l->GetBitset()[0].test(0))
        return 0;

    /* Get the complementary bitset */
    bitset<BITSET_SIZE> I;
    for (int i = 0; i < n; i++) {
        if (!l->GetBitset()[0].test(i))
            I.set(i);
    }

    Vertex* v = l->GetVertex();

    /* Find length of MST on I */
    int MST_length = MST(I, terminals, n);
    
    /* Find terminal in I closest to v */
    int min_dist = INT_MAX;
    int min_index;
    for (int i = 0; i < n; i++) {
        if (I.test(i) && RectDistance(v, terminals[i]) < min_dist) {
            min_dist = RectDistance(v, terminals[i]);
            min_index = i;
        }
    }

    if (I.count() == 1)
        return (MST_length / 2) + min_dist;

    /* Find distance to terminal second closest to v */
    int snd_min_dist = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (I.test(i)           && 
                i != min_index  &&
                RectDistance(v, terminals[i]) < snd_min_dist) {
            snd_min_dist = RectDistance(v, terminals[i]);
        }
    }

    return (MST_length / 2) + (min_dist + snd_min_dist) / 2;
}

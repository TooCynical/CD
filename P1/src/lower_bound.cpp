#include "lower_bound.hpp"

using namespace std;

BoundComputator::BoundComputator(Instance *inst) {
    _underlying_instance = inst;
}

/* Compute the bounding box lower bound for a 
 * a label (i.e. the bounding box lower bound for
 * a Steiner Tree on its complement) given the 
 * terminal set used and the number of total terminals. */ 
int BoundComputator::BBLowerBound(Label *l) {
    /* If root terminal in the terminal set of the label 
     * return 0 */
    if (l->GetBitset()[0].test(0)) {
        return 0;
    }

    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();

    int x_max, x_min;
    int y_max, y_min;
    int z_max, z_min;
    
    /* Set initial values based on vertex in label */
    x_max = x_min = l->GetVertex()->GetX();
    y_max = y_min = l->GetVertex()->GetY();
    z_max = z_min = l->GetVertex()->GetZ();

    /* Loop over all terminals in the complement of the labels
     * terminal set and update values accordingly */
    for (int i = 0; i < n; i++) {
        if (!l->GetBitset()[0].test(i)) {
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
 * on the given subset of terminals using a O(kn^2)-
 * implementation of Prim's Algorithm, where k is the 
 * total number of terminals, and n the number of terminals in I. */
 int BoundComputator::MST(const bitset<BITSET_SIZE> &I) {
    int n_rel_terminals = I.count();

    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();
    
    /* An MST on <= 1 vertex has length 0. */
    if (n_rel_terminals <= 1)
        return 0;
    
    /* First make new references to the relevant terminals */
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
int BoundComputator::MSTLowerBound(Label *l) {
    /* If root terminal in the terminal set of the label 
     * return 0. */
    if (l->GetBitset()[0].test(0))
        return 0;

    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();

    Vertex* v = l->GetVertex();

    /* Get the complementary bitset */
    bitset<BITSET_SIZE> I;
    for (int i = 0; i < n; i++) {
        if (!l->GetBitset()[0].test(i))
            I.set(i);
    }

    /* Find length of MST on I. First check if it was computed already. 
     * If not, compute it and add it to the hash table. */
    int MST_length;
    auto it = _MST_hash.find(l->GetBitset()[0]);
    if (it != _MST_hash.end()) {
        MST_length = it->second;
    } 
    else {
        MST_length = MST(I);
        _MST_hash.insert(make_pair(l->GetBitset()[0], MST_length));
    }
    
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

/* Compute d(I, R-I) for given terminal set I in time O(|I|*|R-I|). */
Result BoundComputator::ComplementDistance(const bitset<BITSET_SIZE> &I,
                                           int &ret_dist,
                                           int &ret_ind) {
    
    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();

    /* Get the coordinates i for which terminals[i] is in I and
     * those for which it is not. */
    vector<int> indices_in_I;
    vector<int> indices_not_in_I;
    for (int i = 0; i < n; i++) {
        if (I.test(i))
            indices_in_I.push_back(i);
        else
            indices_not_in_I.push_back(i);
    }

    if (indices_in_I.size() == 0 || indices_not_in_I.size() == 0) {
        cout << "ERROR: computing d(R, I-R) for empty I or empty I-R. \n";
        exit(1); 
    }

    /* Now find d(I, R-I) by finding d(i, j) for all i in I and 
     * j in R-I. */
    int min = INT_MAX;
    int min_ind;
    for (unsigned int i = 0; i < indices_in_I.size(); i++) {
        for (unsigned int j = 0; j < indices_not_in_I.size(); j++) {
            int k = indices_in_I[i];
            int l = indices_not_in_I[j];
            if (RectDistance(terminals[k], terminals[l]) < min) {
                min = RectDistance(terminals[k], terminals[l]);
                min_ind = l;
            }
        }
    }

    ret_dist = min;
    ret_ind = min_ind;

    return SUCCESS;
}

/* Compute d(v, R-I) for given terminal set I in O(|R-I|). */
Result BoundComputator::VertexComplementDistance(
        const bitset<BITSET_SIZE> &I, 
        Vertex *v, 
        int &ret_dist, 
        int &ret_ind) {

    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();

    if (I.test(0)) {
        cout << "ERROR: computing d(v, I-R) for root in I. \n";
        exit(1); 
    }

    int min = INT_MAX;
    int min_ind;
    for (int i = 0; i < n; i++) {
        if (!I.test(i) && RectDistance(v, terminals[i]) < min) {
            min = RectDistance(v, terminals[i]);
            min_ind = i;
        }
    }

    ret_dist = min;
    ret_ind = min_ind;
    return SUCCESS;
}

/* Return whether value > U(I), the local upper bound for I. 
 * We implement this like this in order to elegantly deal 
 * with the case that U(I) is not yet set. */
bool BoundComputator::CompareToUpperBound(bitset<BITSET_SIZE> &I, int value) {
    /* Attempt to fetch U(I) */
    auto it = _upper_bound_hash.find(I);
    /* U(I) set, compare it to value */
    if (it != _upper_bound_hash.end()) {
        return (value > it->second.first);
    } 
    /* U(I) not set (i.e. U(I) = infty) */
    else {
        return false;
    }
}

/* Fetch d(I, R-I) from the hash table or compute it if not yet set */
Result BoundComputator::GetComplementDistance(const bitset<BITSET_SIZE> &I,
                                              int &ret_dist,
                                              int &ret_ind) {
    /* Attempt to fetch d(I, R-I). */
    auto it = _distance_hash.find(I);
    /* d(I, R-I) set, return it. */
    if (it != _distance_hash.end()) {
        ret_dist = it->second.first;
        ret_ind = it->second.second;
    } 
    /* R-I not set, compute it, set it, and return it. */
    else {
        ComplementDistance(I, ret_dist, ret_ind);
        _distance_hash.insert(make_pair(I, make_pair(ret_dist, ret_ind)));
    }
    return SUCCESS;
}

/* Update U(I) provided that the given label (v, I)
 * minimizes l(v,I) + lb(v, I) for labels in N and 
 * v is not in I. */
Result BoundComputator::UpdateUpperBound(Label *l) {
    
    bitset<BITSET_SIZE> I = *(l->GetBitset());

    /* Compute l(v,I) + min (d(I, R-I), d(v, R-I)) and the index 
     * of the terminal in R- I closest to v or closest to I,
     * respectively. */
    int vertex_dist, vertex_ind, compl_dist, compl_ind, min_ind;
    GetComplementDistance(I, compl_dist, compl_ind);
    VertexComplementDistance(I, l->GetVertex(), vertex_dist, vertex_ind);
    if (vertex_dist < compl_dist)
        min_ind = vertex_ind;
    else
        min_ind = compl_ind;
    int value = l->GetL() + min(compl_dist, vertex_dist);

    /* Attempt to fetch A(I) */
    auto it = _upper_bound_hash.find(I);
    /* If A(I) set, compare U(I) to value and update U(I) 
     * and S(I) if needed. */
    if (it != _upper_bound_hash.end()) {
        if (value < it->second.first) {
            _upper_bound_hash[I].first = value;   
            bitset<BITSET_SIZE> new_S;
            new_S.set(min_ind);
            _upper_bound_hash[I].second = new_S;
        }
    } 
    /* If A(I) not set (i.e. U(I) = infty) then set it. */
    else {
        bitset<BITSET_SIZE> new_S;
        new_S.set(min_ind);
        _upper_bound_hash.insert(make_pair(I, make_pair(value, new_S)));
    }

    return SUCCESS;
}

Result BoundComputator::MergeUpperBound(const bitset<BITSET_SIZE> &I,
                                        const bitset<BITSET_SIZE> &J) {

    /* If either A(I) or A(J) is not set we should not do anything. */
    auto it_I = _upper_bound_hash.find(I);
    auto it_J = _upper_bound_hash.find(J);
    if (it_I == _upper_bound_hash.end() || it_J == _upper_bound_hash.end())
        return FAIL;

    bitset<BITSET_SIZE> IJ = I | J;

    /* Check if S(I) \cap J or I \cap S(J) is empty. If not, nothing 
     * is to be done. */
    if ((_upper_bound_hash[I].second & J).any() && 
        (_upper_bound_hash[J].second & I).any())
        return FAIL;

    /* Find U(I) + U(J) and (S(I) u S(J)) - (I u J) */
    int sum_U = _upper_bound_hash[I].first + _upper_bound_hash[J].first;

    /* First find union */
    bitset<BITSET_SIZE> new_S = _upper_bound_hash[I].second | 
                                _upper_bound_hash[J].second;

    /* Then remove unwanted elements. */ 
    new_S = (new_S & ~I) & ~J;

    /* Attempt to fetch A(IJ) */
    auto it = _upper_bound_hash.find(IJ);
    /* A(I) set. */
    if (it != _upper_bound_hash.end()) {
        /* Check if U(I) + U(J) < U(IJ) and update U(IJ) if so. */
        if (sum_U < it->second.first) {
            it->second.first = sum_U;
            it->second.second = new_S;
        }
    } 
    /* A(IJ) not set. Set it.*/
    else {
        _upper_bound_hash.insert(make_pair(IJ, make_pair(sum_U, new_S)));
    }

    return SUCCESS;
}

/* Return whether the terminals given by I intersect with the 
 * set of vertices given by S. */
bool BoundComputator::SetIntersectsTerminalSet(const bitset<BITSET_SIZE> &I, 
                                              set<int> &S) {
    Vertex **terminals = _underlying_instance->GetTerminals();
    int n = _underlying_instance->GetNTerminals();


    for (int i = 0; i < n; i++) {
        if (I.test(i)) {
            if (S.count(terminals[i]->GetId()) > 0)
                return true;
        }
    }
    return false;
}
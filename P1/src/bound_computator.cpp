/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * bound_computator.cpp
 *
 *
 * Implementation of functionality described in bound_computator.hpp.
 */

#include "bound_computator.hpp"

using namespace std;

BoundComputator::BoundComputator(Instance *inst, BoundOptions *opts) {
    _underlying_instance = inst;
    _n_terminals = inst->GetNTerminals();
    _terminals = inst->GetTerminals();

    /* Extract lower bound options */
    if (opts != NULL) {
        _use_BB_lower_bound = opts->_use_BB_lower_bound;
        _use_onetree_lower_bound = opts->_use_onetree_lower_bound;
        free(opts);
    }
    /* By default use BB and OneTree lower bounds. */
    else {
        _use_BB_lower_bound = true;
        _use_onetree_lower_bound = true;    
    }
}

PerimeterCoords BoundComputator::Perimeter(const bitset<BITSET_SIZE> &I) {    
    /* Empty terminal set. */
    if (!I.any()) {
        cout << "ERROR: Perimeter called for empty terminal set\n";        
        exit(1);
    }

    int x_max, x_min;
    int y_max, y_min;
    int z_max, z_min;
    
    x_min = y_min = z_min = MAX_COORD * (1 + DOUBLE_INPUT_COORDS);
    x_max = y_max = z_max = 0;

    /* Loop over all terminals in the complement of the labels
     * terminal set and update values accordingly */
    for (int i = 0; i < _n_terminals; i++) {
        if (!I.test(i)) {
            if (_terminals[i]->GetX() < x_min)
                x_min = _terminals[i]->GetX();
            if (_terminals[i]->GetX() > x_max)
                x_max = _terminals[i]->GetX();
            if (_terminals[i]->GetY() < y_min)
                y_min = _terminals[i]->GetY();
            if (_terminals[i]->GetY() > y_max)
                y_max = _terminals[i]->GetY();
            if (_terminals[i]->GetZ() < z_min)
                z_min = _terminals[i]->GetZ();
            if (_terminals[i]->GetZ() > z_max)
                z_max = _terminals[i]->GetZ();
        }
    }

    PerimeterCoords ret;
    ret._x_max = x_max;
    ret._x_min = x_min;
    ret._y_max = y_max;
    ret._y_min = y_min;
    ret._z_max = z_max;
    ret._z_min = z_min;
    return ret;
}

int BoundComputator::BBLowerBound(Label *l) {
    bitset<BITSET_SIZE> I = l->GetBitset();

    /* Try to fetch the perimeter from the hash table, 
     * compute it otherwise. */
    PerimeterCoords I_perimeter;
    auto it = _perimeter_hash.find(I);
    if (it != _perimeter_hash.end()) {
        I_perimeter = it->second;
    } 
    else {
        I_perimeter = Perimeter(I);
        _perimeter_hash.insert(make_pair(I, I_perimeter));
    }

    int x_max = max(l->GetVertex()->GetX(), I_perimeter._x_max);
    int x_min = min(l->GetVertex()->GetX(), I_perimeter._x_min);
    int y_max = max(l->GetVertex()->GetY(), I_perimeter._y_max);
    int y_min = min(l->GetVertex()->GetY(), I_perimeter._y_min);
    int z_max = max(l->GetVertex()->GetZ(), I_perimeter._z_max);
    int z_min = min(l->GetVertex()->GetZ(), I_perimeter._z_min);

    return (x_max - x_min) + (y_max - y_min) + (z_max - z_min);
}

int BoundComputator::MST(const bitset<BITSET_SIZE> &I) {
    
    vector<Vertex*> terminals_in_I;
    for (int i = 0; i < _n_terminals; i++){
        if (I.test(i))
            terminals_in_I.push_back(_terminals[i]);
    }

    int n_terminals_in_I = terminals_in_I.size();

    /* An MST on <= 1 vertex has length 0. */
    if (n_terminals_in_I <= 1)
        return 0;
 
    /* Array containing cheapest cost of a connection to each vertex. 
     * Initially set all values to infinity except for costs[0]. */
    vector<int> costs(n_terminals_in_I, INT_MAX);
    costs[0] = 0;

    /* Array indicating which terminals are already in the tree,
     * initially add the first one. */
    vector<bool> in_tree(n_terminals_in_I, false);

    int min = INT_MAX;
    int min_index = 0;
    int tree_cost = 0;
    int cur_distance;

    for (int j = 0; j < n_terminals_in_I; j++) {
        min = INT_MAX;
        /* Find terminal with minimum cost and add it to tree. */
        for (int i = 0; i < n_terminals_in_I; i++) {
            if (costs[i] < min && !in_tree[i]) {
                min = costs[i];
                min_index = i;
            }
        }

        /* Update terminal costs and total tree cost. */
        tree_cost += costs[min_index];
        in_tree[min_index] = true;
        for (int i = 0; i < n_terminals_in_I; i++) {
            cur_distance = RectDistance(terminals_in_I[i], 
                                        terminals_in_I[min_index]);
            if (cur_distance < costs[i] && i != min_index) {
                costs[i] = cur_distance;
            }
        }
    }

    return tree_cost;
 }

int BoundComputator::OneTreeLowerBound(Label *l) {

    Vertex* v = l->GetVertex();

    /* Get the complementary bitset. */
    bitset<BITSET_SIZE> I = l->GetBitset();
    bitset<BITSET_SIZE> C = ~l->GetBitset();

    /* Find length of MST on C. First check if it was computed already. 
     * If not, compute it and add it to the hash table. */
    int MST_length;
    auto it = _MST_hash.find(I);
    if (it != _MST_hash.end()) {
        MST_length = it->second;
    } 
    else {
        MST_length = MST(C);
        _MST_hash.insert(make_pair(I, MST_length));
    }
    
    /* Find terminal in C closest to v and second closest to v. */
    int min_dist = INT_MAX;
    int snd_min_dist = INT_MAX;
    int min_index;
    int cur_dist;
    for (int i = 0; i < _n_terminals; i++) {
        if (C.test(i)) {
            if ((cur_dist = RectDistance(v, _terminals[i])) < min_dist) {
                snd_min_dist = min_dist;
                min_dist = cur_dist;
                min_index = i;
            }
            if (i != min_index && cur_dist < snd_min_dist)
                snd_min_dist = cur_dist;
        }
    }

    /* Case for |C| = 1: */
    if (_n_terminals - I.count() == 1)
        return (MST_length / 2) + min_dist;

    /* If snd_min_dist is still infty at this point something
     * is very wrong. */
    if (snd_min_dist == INT_MAX) {
        cout << "ERROR: snd_min_dist infty!\n";
        exit(1);
    }

    /* Case for |C| > 1: */
    return (MST_length / 2) + (min_dist + snd_min_dist) / 2;
}

Result BoundComputator::ComplementDistance(const bitset<BITSET_SIZE> &I,
                                           int &ret_dist,
                                           int &ret_ind) {
    
    /* Get the coordinates i for which terminals[i] is in I and
     * those for which it is not. */
    vector<int> indices_in_I;
    vector<int> indices_not_in_I;
    for (int i = 0; i < _n_terminals; i++) {
        if (I.test(i))
            indices_in_I.push_back(i);
        else
            indices_not_in_I.push_back(i);
    }

    /* I and R-I should never be empty! */
    if (indices_in_I.size() == 0 || indices_not_in_I.size() == 0) {
        cout << "ERROR: computing d(R, I-R) for empty I or empty I-R. \n";
        exit(1); 
    }

    /* Now find d(I, R-I) by finding d(i, j) for all i in I and 
     * j in R-I. */
    int min = INT_MAX;
    int min_ind;
    int cur_dist;
    for (unsigned int i = 0; i < indices_in_I.size(); i++) {
        for (unsigned int j = 0; j < indices_not_in_I.size(); j++) {
            int k = indices_in_I[i];
            int l = indices_not_in_I[j];
            if ((cur_dist = RectDistance(_terminals[k], _terminals[l])) 
                    < min) {
                min = cur_dist;
                min_ind = l;
            }
        }
    }

    ret_dist = min;
    ret_ind = min_ind;

    return SUCCESS;
}

Result BoundComputator::VertexComplementDistance(
        const bitset<BITSET_SIZE> &I, 
        Vertex *v, 
        int &ret_dist, 
        int &ret_ind) {

    int min = INT_MAX;
    int min_ind;
    int cur_dist;
    /* Loop over all terminals in R-I to find the closest one to v. */
    for (int i = 0; i < _n_terminals; i++) {
        if (!I.test(i) && (cur_dist = RectDistance(v, _terminals[i])) < min) {
            min = cur_dist;
            min_ind = i;
        }
    }

    ret_dist = min;
    ret_ind = min_ind;
    return SUCCESS;
}

bool BoundComputator::CompareToUpperBound(Label* l) {
    bitset<BITSET_SIZE> I = l->GetBitset();

    /* Attempt to fetch U(I) from the hash table */
    auto it = _upper_bound_hash.find(I);
    /* If A(I) is set, compare U(I) to value. */
    if (it != _upper_bound_hash.end()) {
        return (l->GetL() > it->second.first);
    } 
    /* If A(I) is not set (i.e. U(I) = infty) we 
     * return false. */
    else
        return false;
}

Result BoundComputator::GetComplementDistance(const bitset<BITSET_SIZE> &I,
                                              int &ret_dist,
                                              int &ret_ind) {
    /* Attempt to fetch d(I, R-I) from the hash table. */
    auto it = _distance_hash.find(I);
    /* If d(I, R-I) set return it. */
    if (it != _distance_hash.end()) {
        ret_dist = it->second.first;
        ret_ind = it->second.second;
    } 
    /* If R-I not set, compute it, insert it in the hash table,
     * and return it. */
    else {
        ComplementDistance(I, ret_dist, ret_ind);
        _distance_hash.insert(make_pair(I, make_pair(ret_dist, ret_ind)));
    }
    return SUCCESS;
}

Result BoundComputator::UpdateUpperBound(Label *l) {
    
    bitset<BITSET_SIZE> I = l->GetBitset();

    /* Compute l(v,I) + min (d(I, R-I), d(v, R-I)) and the index 
     * of the terminal in R-I closest to v or closest to I,
     * accordingly. */
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

    /* Find U(I) + U(J) and (S(I) u S(J)) - (I u J). */
    int sum_U = _upper_bound_hash[I].first + _upper_bound_hash[J].first;

    /* First find union then remove I and J. */
    bitset<BITSET_SIZE> new_S = _upper_bound_hash[I].second | 
                                _upper_bound_hash[J].second;
    new_S = (new_S & ~I) & ~J;

    /* Attempt to fetch A(IuJ). */
    auto it = _upper_bound_hash.find(IJ);
    /* If A(I u J) set: Check if U(I) + U(J) < U(IuJ) 
     * and update U(I u J) and S(I u J) if so. */
    if (it != _upper_bound_hash.end()) {
        if (sum_U < it->second.first) {
            it->second.first = sum_U;
            it->second.second = new_S;
        }
    } 
    /* If A(I u J) not set, set it.*/
    else {
        _upper_bound_hash.insert(make_pair(IJ, make_pair(sum_U, new_S)));
    }

    return SUCCESS;
}

int BoundComputator::GetLowerBound(Label *l) {
    if (_use_BB_lower_bound && _use_onetree_lower_bound)
        return max(BBLowerBound(l), OneTreeLowerBound(l));
    if (_use_BB_lower_bound)
        return BBLowerBound(l);
    if (_use_onetree_lower_bound)
        return OneTreeLowerBound(l);
    return 0;
}

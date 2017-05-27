#include "lower_bound.hpp"

using namespace std;

BoundComputator::BoundComputator(Instance *inst) {
    _underlying_instance = inst;
    _n_terminals = inst->GetNTerminals();
    _terminals = inst->GetTerminals();

    _use_BB_lower_bound = true;
    _use_onetree_lower_bound = true;
}

int BoundComputator::BBLowerBound(Label *l) {
    /* If root terminal in the terminal set of the label 
     * return 0 and print a warning. */
    if (l->GetBitset().test(0)) {
        cout << "WARNING: BBLowerBound called for root in I!\n";
        return 0;
    }

    int x_max, x_min;
    int y_max, y_min;
    int z_max, z_min;
    
    /* Set initial values based on vertex in label */
    x_max = x_min = l->GetVertex()->GetX();
    y_max = y_min = l->GetVertex()->GetY();
    z_max = z_min = l->GetVertex()->GetZ();

    /* Loop over all terminals in the complement of the labels
     * terminal set and update values accordingly */
    for (int i = 0; i < _n_terminals; i++) {
        if (!l->GetBitset().test(i)) {
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
    return (x_max - x_min) + (y_max - y_min) + (z_max - z_min);
}

int BoundComputator::MST(const bitset<BITSET_SIZE> &I) {
    int n_rel_terminals = I.count();
    
    /* An MST on <= 1 vertex has length 0. */
    if (n_rel_terminals <= 1)
        return 0;
    
    /* First make new references to the relevant terminals */
    Vertex **rel_terminals = (Vertex**) calloc(n_rel_terminals,
                                                sizeof(Vertex*));
    int count = 0;
    for (int i = 0; i < _n_terminals; i++) {
        if (I.test(i)) {
            rel_terminals[count] = _terminals[i];
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
    int cur_distance;

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
            cur_distance = RectDistance(rel_terminals[i], 
                                        rel_terminals[min_index]);
            if (cur_distance < costs[i] && i != min_index) {
                costs[i] = cur_distance;
            }
        }
    }

    /* Reclaim memory. */
    free(rel_terminals);
    free(costs);
    free(in_tree);

    return tree_cost;
 }

int BoundComputator::OneTreeLowerBound(Label *l) {
    /* If root terminal in the terminal set of the label 
     * return 0 and print a warning. */
    if (l->GetBitset().test(0)) {
        cout << "WARNING: OneTreeLowerBound called for root in I!\n";
        return 0;
    }

    Vertex* v = l->GetVertex();

    /* Get the complementary bitset. */
    bitset<BITSET_SIZE> I;
    for (int i = 0; i < _n_terminals; i++) {
        if (!l->GetBitset().test(i))
            I.set(i);
    }

    /* Find length of MST on I. First check if it was computed already. 
     * If not, compute it and add it to the hash table. */
    int MST_length;
    auto it = _MST_hash.find(l->GetBitset());
    if (it != _MST_hash.end()) {
        MST_length = it->second;
    } 
    else {
        MST_length = MST(I);
        _MST_hash.insert(make_pair(l->GetBitset(), MST_length));
    }
    
    /* Find terminal in I closest to v. */
    int min_dist = INT_MAX;
    int min_index;
    int cur_dist;
    for (int i = 0; i < _n_terminals; i++) {
        if (I.test(i) && 
            (cur_dist = RectDistance(v, _terminals[i])) < min_dist) {
            min_dist = cur_dist;
            min_index = i;
        }
    }

    /* If I has only one terminal we need not find second closest
     * terminal to v */
    if (I.count() == 1)
        return (MST_length / 2) + min_dist;

    /* Find distance to terminal in I second closest to v */
    int snd_min_dist = INT_MAX;

    for (int i = 0; i < _n_terminals; i++) {
        if (I.test(i) && i != min_index && 
            (cur_dist = RectDistance(v, _terminals[i])) < snd_min_dist) {
            snd_min_dist = cur_dist;
        }
    }

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

    /* The root should never be in I. */
    if (I.test(0)) {
        cout << "ERROR: computing d(v, I-R) for root in I. \n";
        exit(1); 
    }

    int min = INT_MAX;
    int min_ind;
    /* Loop over all terminals in R-I to find the closest one to v. */
    for (int i = 0; i < _n_terminals; i++) {
        if (!I.test(i) && RectDistance(v, _terminals[i]) < min) {
            min = RectDistance(v, _terminals[i]);
            min_ind = i;
        }
    }

    ret_dist = min;
    ret_ind = min_ind;
    return SUCCESS;
}

bool BoundComputator::CompareToUpperBound(const bitset<BITSET_SIZE> &I, 
                                          int value) {
    /* Attempt to fetch U(I) from the hash table */
    auto it = _upper_bound_hash.find(I);
    /* If A(I) is set, compare U(I) to value. */
    if (it != _upper_bound_hash.end()) {
        return (value > it->second.first);
    } 
    /* If A(I) is not set (i.e. U(I) = infty) we 
     * return false. */
    else {
        return false;
    }
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

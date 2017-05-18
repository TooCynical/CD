#include "label.hpp"

using namespace std;

Label::Label(Vertex *v, bitset<BITSET_SIZE> I) {
    _v = v;
    _I = I;

    _l_set = false;
    _in_P = false;
    _been_in_Q = false;
    _lower_bound_set = false;
}

Result Label::SetL(int l) {
    _l_set = true;
    _l = l;
    return SUCCESS;
}

Vertex *Label::GetVertex() const {
    return _v;
}

bitset<BITSET_SIZE> *Label::GetBitset() {
    return &_I;
}

bool Label::IsLSet() const { return _l_set; }
bool Label::IsLowerBoundSet() const { return _lower_bound_set; }

int Label::GetL() const {
    if (_l_set)
        return _l;
    else {
        cout << "WARNING: GetL called when l not set!\n";
        return -1;
    }
}

int Label::GetLowerBound() const {
    if (_lower_bound_set)
        return _lower_bound;
    else {
        cout << "WARNING: GetLowerBound called when _lower_bound not set!\n";
        return -1;
    }
}

/* Compute a lower bound for the complement of this label */
Result Label::SetLowerBound(int l) {
    _lower_bound = 0;
    _lower_bound_set = true;
    return SUCCESS;
}

Result Label::SetInP() { 
    _in_P = true; 
    return SUCCESS;
}
Result Label::SetBeenInQ() {
    _been_in_Q = true;
    return SUCCESS;
}

bool Label::IsInP() const { return _in_P; }
bool Label::BeenInQ() const { return _been_in_Q; }

void Label::Print() {
    cout << "Vertex: \n";
    _v->Print();
    cout << "Bitset: " << _I << "\n";
}

/* A label is smaller than another label if it has BIGGER l-value */
bool operator<(const Label& a, const Label& b) {
    if (!a.IsLSet() || !b.IsLSet()) {
        cout << "WARNING: Comparing Labels with unset l-values!\n";
        return true;
    }
    if (!a.IsLowerBoundSet() || !b.IsLowerBoundSet()) {
        cout << "WARNING: Comparing Labels with unset lower bounds!\n";
        return true;
    }
    return a.GetL() + a.GetLowerBound() > b.GetL() + b.GetLowerBound();
}

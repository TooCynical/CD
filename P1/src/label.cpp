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

/* Getters / Setters */

bool Label::IsLSet() const { return _l_set; }
bool Label::IsLowerBoundSet() const { return _lower_bound_set; }
bool Label::IsInP() const { return _in_P; }
bool Label::BeenInQ() const { return _been_in_Q; }
Vertex *Label::GetVertex() const { return _v; }
bitset<BITSET_SIZE> *Label::GetBitset() { return &_I; }


Result Label::SetL(int l) {
    _l_set = true;
    _l = l;
    return SUCCESS;
}

Result Label::SetLowerBound(int l) {
    _lower_bound_set = true;
    _lower_bound = l;
    return SUCCESS;
}

/* Set _in_P to true. Return
 * FAIL if _in_P was already true */
Result Label::SetInP() {
    if (!_in_P) {
        _in_P = true; 
        return SUCCESS;
    }
    else {
        return FAIL;
    }
}
Result Label::SetBeenInQ() {
    _been_in_Q = true;
    return SUCCESS;
}

/* IO-functions for testing */
void Label::Print() {
    cout << "Vertex: \n";
    _v->Print();
    cout << "Bitset: " << _I << "\n";
}

/* A label is smaller than another label if it has BIGGER l-value */
bool operator<(const Label& a, const Label& b) {
    if (!a.IsLSet() || !b.IsLSet()) {
        cout << "WARNING: Comparing Labels with unset l-values!\n";
        exit(1);
    }
    if (!a.IsLowerBoundSet() || !b.IsLowerBoundSet()) {
        cout << "WARNING: Comparing Labels with unset lower bounds!\n";
        exit(1);
    }
    return a.GetL() + a.GetLowerBound() > b.GetL() + b.GetLowerBound();
}

#include "label.hpp"

using namespace std;

Label::Label(Vertex *v, bitset<BITSET_SIZE> I) {
    _v = v;
    _I = I;
    _l_set = false;
    _in_P = false;
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

int Label::GetL() const {
    if (_l_set)
        return _l;
    else {
        cout << "WARNING: GetL called when l not set!\n";
        return -1;
    }

}

Result Label::SetInP() { 
    _in_P = true; 
    return SUCCESS;
}

bool Label::IsInP() const { return _in_P; }


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
    return a.GetL() > b.GetL();
}

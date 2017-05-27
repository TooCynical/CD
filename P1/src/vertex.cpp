#include "vertex.hpp"

using namespace std;

Vertex::Vertex(int x, int y, int z) {
    _x = x;
    _y = y;
    _z = z;
    _n_neigh = 0;
    _n_labels = 0;
    _is_root = false;
}

Vertex::~Vertex() {
    for (int i = 0; i < _n_labels; i++) {
        delete _labels[i];
    }
    _label_hash.clear();
}

Result Vertex::AddNeighbour(Vertex* w) {
    _neigh.push_back(w);
    _n_neigh ++;
    return SUCCESS;
}

Result Vertex::AddLabel(Label *l) {
    _labels.push_back(l);
    _label_hash.insert(make_pair(*l->GetBitset(), _n_labels));
    _n_labels ++;
    return SUCCESS;
}

bool Vertex::HasCoords(int x, int y, int z) const {
    return (_x == x && _y == y && _z == z);
}

const int& Vertex::GetX() const { return _x; }
const int& Vertex::GetY() const { return _y; }
const int& Vertex::GetZ() const { return _z; }
const int& Vertex::GetId() const { return _id; }

bool Vertex::IsRoot() const { return _is_root; }

Result Vertex::SetId(int id) {
    if (id < 0)
        return FAIL;
    else {
        if (_id >= 0) {
            _id = id;
            return SUCCESS;
        }
        else
            return FAIL;
    }
}

Result Vertex::SetRoot() { 
    if (_is_root)
        return FAIL;
    else {
        _is_root = true; 
        return SUCCESS;
    }
}

const int &Vertex::GetNNeighbours() const { return _n_neigh; }

const vector<Vertex*> &Vertex::GetNeighbours() const { return _neigh; }

const vector<Label*> &Vertex::GetLabels() const { return _labels; }

Label *Vertex::GetLabelByBitset(const bitset<BITSET_SIZE> &I) const {
    auto it = _label_hash.find(I);
    if (it != _label_hash.end()) {
        return _labels[it->second];
    }
    else {
        return NULL;
    }
}

bool Vertex::operator==(const Vertex& other) { 
    return _id == other.GetId();
}

void Vertex::Print() const {
    cout << _x << ", " << _y << ", " << _z << "\n";
}

void Vertex::PrintNeighbours() const {
    for (int i = 0; i < _n_neigh; i++)
        _neigh[i]->Print();
}

int RectDistance(Vertex *v, Vertex *w) {
    return  (abs(v->GetX() - w->GetX()) +
            abs(v->GetY() - w->GetY()) +
            abs(v->GetZ() - w->GetZ()));
}
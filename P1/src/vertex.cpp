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

void Vertex::Print() {
    cout << _x << ", " << _y << ", " << _z << "\n";
}

void Vertex::PrintNeighbours() {
    for (int i = 0; i < _n_neigh; i++)
        _neigh[i]->Print();
}

int Vertex::getX() { return _x; }
int Vertex::getY() { return _y; }
int Vertex::getZ() { return _z; }

// bool Vertex::operator<(const Vertex& other) const {
//     if (_x < other._x)
//         return true;
//     if (_y < other._y)
//         return true;
//     if (_z < other._z)
//         return true;
//     return false;
// }

bool Vertex::IsRoot() const { return _is_root; }
Result Vertex::SetRoot() { 
    _is_root = true; 
    return SUCCESS;
}

/* Return whether the vertex is located at given coordinates */
bool Vertex::HasCoords(int x, int y, int z) {
    return (_x == x && _y == y && _z == z);
}

Result Vertex::AddNeighbour(Vertex* w) {
    _neigh.push_back(w);
    _n_neigh ++;
    return SUCCESS;
}

/* Add label to _labels and add its index to the _label_hash. */
Result Vertex::AddLabel(Label *l) {
    _labels.push_back(l);
    _label_hash.insert(make_pair(*l->GetBitset(), _n_labels));
    _n_labels ++;
    return SUCCESS;
}

int Vertex::GetNNeigh() { return _n_neigh; }
vector<Vertex*> *Vertex::GetNeigh() { return &_neigh; }
vector<Label*> *Vertex::GetLabels() { return &_labels; }
unordered_map<bitset<BITSET_SIZE>, int> *Vertex::GetLabelHash() { return &_label_hash; }
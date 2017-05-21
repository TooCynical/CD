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

/* Add w to _neigh and increase number of neighbours. */
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

/* Return whether the vertex is located at given coordinates */
bool Vertex::HasCoords(int x, int y, int z) {
    return (_x == x && _y == y && _z == z);
}

/* Getters / Setters */
int Vertex::GetX() { return _x; }
int Vertex::GetY() { return _y; }
int Vertex::GetZ() { return _z; }

bool Vertex::IsRoot() const { return _is_root; }
Result Vertex::SetRoot() { 
    _is_root = true; 
    return SUCCESS;
}

int Vertex::GetNNeigh() { return _n_neigh; }
vector<Vertex*> *Vertex::GetNeigh() { return &_neigh; }
vector<Label*> *Vertex::GetLabels() { return &_labels; }
unordered_map<bitset<BITSET_SIZE>, int> *Vertex::GetLabelHash() {
    return &_label_hash; 
}

/* IO-functions for testing */
void Vertex::Print() {
    cout << _x << ", " << _y << ", " << _z << "\n";
}

void Vertex::PrintNeighbours() {
    for (int i = 0; i < _n_neigh; i++)
        _neigh[i]->Print();
}

/* Return the rectilinear distance between two vertices */
int RectDistance(Vertex *v, Vertex *w) {
    return  (abs(v->GetX() - w->GetX()) +
            abs(v->GetY() - w->GetY()) +
            abs(v->GetZ() - w->GetZ()));
}
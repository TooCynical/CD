#include "instance.hpp"

using namespace std;

Result Instance::SetXYZ(int **term_locs) {
    for (int i = 0; i < _n_terminals; i++) {
        _x_values.insert(term_locs[i][0]);
        _y_values.insert(term_locs[i][1]);
        _z_values.insert(term_locs[i][2]);
    }

    _n_x_values = _x_values.size();
    _n_y_values = _y_values.size();
    _n_z_values = _z_values.size();

    return SUCCESS;
}

Result Instance::SetVertices() {
    set<int>::iterator it_x, it_y, it_z;
    
    for (it_x = _x_values.begin(); it_x != _x_values.end(); it_x++) {
        for (it_y = _y_values.begin(); it_y != _y_values.end(); it_y++) {
            for (it_z = _z_values.begin(); it_z != _z_values.end(); it_z++) {
                _V.push_back(new Vertex(*it_x, *it_y, *it_z));
            }
        }
    }
    return SUCCESS;
}

Result Instance::SetHananGrid() {
    _hanan_grid = (Vertex****) calloc(_n_x_values, sizeof(Vertex***));

    for (int i = 0; i < _n_x_values; i++) {
        _hanan_grid[i] = (Vertex***) calloc(_n_y_values, sizeof(Vertex**));
        for (int j = 0; j < _n_y_values; j++) {
            _hanan_grid[i][j] = (Vertex**) calloc(_n_z_values, sizeof(Vertex*));
            for (int k = 0; k < _n_z_values; k++) {
                _hanan_grid[i][j][k] = _V[i*_n_y_values*_n_z_values + j*_n_z_values + k];
            }
        }
    }
    return SUCCESS;
}

Result Instance::SetNeighbours() {
    /* Making use of the ordering of the vertices in _hanan_grid,
     * we can add all neighbours easily. */
    for (int i = 0; i < _n_x_values; i++) {
        for (int j = 0; j < _n_y_values; j++) {
            for (int k = 0; k < _n_z_values; k++) {
                /* Add all grid-adjacent vertices as neighbours. */
                Vertex *v = _hanan_grid[i][j][k];
                if (i - 1 >= 0)
                    v->AddNeighbour(_hanan_grid[i-1][j][k]);
                if (i + 1 < _n_x_values)
                    v->AddNeighbour(_hanan_grid[i+1][j][k]);
                if (j - 1 >= 0)
                    v->AddNeighbour(_hanan_grid[i][j-1][k]);
                if (j + 1 < _n_y_values)
                    v->AddNeighbour(_hanan_grid[i][j+1][k]);
                if (k - 1 >= 0)
                    v->AddNeighbour(_hanan_grid[i][j][k-1]);
                if (k + 1 < _n_z_values)
                    v->AddNeighbour(_hanan_grid[i][j][k+1]);
            }
        }
    }
    return SUCCESS;
}

Result Instance::SetTerminals(int **term_locs) {
    _terminals = (Vertex**) calloc(_n_terminals, sizeof(Vertex*));
    
    /* Simply loop over all vertices and see if their coordinates 
     * correspond to those of a terminal. This is rather inefficent. */
    for (int i = 0; i < _n_terminals; i++) {
        for(int j = 0; j < _n_vertices; j++)
        if (_V[j]->HasCoords(term_locs[i][0], term_locs[i][1], term_locs[i][2]))
            _terminals[i] = _V[j];
    }
    return SUCCESS;
}

Result Instance::SortTerminals() {
    /* Find center of mass */
    int x_sum, y_sum, z_sum;
    x_sum = y_sum = z_sum = 0;
    for (int i = 0; i < _n_terminals; i++) {
        x_sum += _terminals[i]->GetX();
        y_sum += _terminals[i]->GetY();
        z_sum += _terminals[i]->GetZ();
    }
    int x_center = x_sum / _n_terminals;
    int y_center = y_sum / _n_terminals;
    int z_center = z_sum / _n_terminals;
    Vertex *center = new Vertex(x_center, y_center, z_center);

    /* Find terminal closed to center. */
    int min_index = 0;
    int min = RectDistance(_terminals[0], center);
    for (int i = 0; i < _n_terminals; i++) {
        if (RectDistance(_terminals[i], center) < min) {
            min = RectDistance(_terminals[i], center);
            min_index = i;
        }
    }
    delete center;

    /* Swap two terminals */
    Vertex *temp = _terminals[0];
    _terminals[0] = _terminals[min_index];
    _terminals[min_index] = temp;

    return SUCCESS;
}

Result Instance::SetIds() {
    for (int i = 0; i < _n_terminals; i++) {
        _V[i]->SetId(i);
    }
    return SUCCESS;
}

Instance::Instance(int n, int **term_locs) {
    _n_terminals = n;
    SetXYZ(term_locs);
    
    SetVertices();
    _n_vertices = _V.size();

    SetHananGrid();
    SetNeighbours();

    SetTerminals(term_locs);
    SortTerminals();

    SetIds();
}

Instance::~Instance() {
    /* Delete all vertices */
    for (int i = 0; i < _n_vertices; i++) {
        delete _V[i];
    }

    /* Delete the Hanan grid */
    for (int i = 0; i < _n_x_values; i++) {
        for (int j = 0; j < _n_y_values; j++) {
            free(_hanan_grid[i][j]);
        }
        free(_hanan_grid[i]);
    }
    free(_hanan_grid);
    free(_terminals);
}

const int &Instance::GetNTerminals() const { return _n_terminals; }

const int &Instance::GetNVertices() const { return _n_vertices; }

const vector<Vertex*> &Instance::GetVertices() const { return _V; }

Vertex **Instance::GetTerminals() { return _terminals; }

void Instance::PrintTerminals() {
    for (int i = 0; i < _n_terminals; i++)
        _terminals[i]->Print();
}

void Instance::PrintVertices() {
    for (int i = 0; i < _n_vertices; i++)
        _V[i]->Print();
}

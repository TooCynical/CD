/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * solver.cpp
 *
 */

 #include "solver.hpp"
 #include "directed_acyclic_graph.hpp"

using namespace std;

Floorplan::Floorplan(const Instance &inst, 
                     size_t width,
                     size_t height,
                     vector<size_t> x_coords,
                     vector<size_t> y_coords,
                     Origin ori) : 
                    _inst(inst), 
                    _n(inst.n_rectangles()),
                    _width(width),
                    _height(height),
                    _area(width * height)
{
    if (set_coords(x_coords, y_coords, ori) == FAIL)
        cout << "Floorplan: Warning failed to set coords" << endl;
}

Result Floorplan::set_coords(vector<size_t> x_coords,
                             vector<size_t> y_coords, 
                             Origin ori) 
{
    if (x_coords.size() != _n || y_coords.size() != _n)
        return FAIL;

    _x_coords = vector<size_t>(_n, 0);
    _y_coords = vector<size_t>(_n, 0);

    for (size_t i = 0; i < _n; i++) {
        switch(ori) {
            case UPPER_RIGHT:
                _x_coords[i] = subtract(x_coords[i], 
                                       _inst.rectangles()[i].width());
                _y_coords[i] = subtract(y_coords[i], 
                                        _inst.rectangles()[i].height());
                break;
            case UPPER_LEFT:
                _x_coords[i] = x_coords[i];
                _y_coords[i] = subtract(y_coords[i], 
                                        _inst.rectangles()[i].height());
                break;
            case LOWER_RIGHT:
                _x_coords[i] = subtract(x_coords[i],
                                        _inst.rectangles()[i].width());
                _y_coords[i] = y_coords[i];
                break;
            case LOWER_LEFT:
                _x_coords[i] = x_coords[i];
                _y_coords[i] = y_coords[i];
                break;
            default:
                cout << "Floorplan: Warning no valid orientation given" << endl;
                return FAIL;
        }
    }
    return SUCCESS;
}

Result Floorplan::verify() {
    for (size_t i = 0; i < _n; i++) {
        for (size_t j = i+1; j < _n; j++) {
            if (intersect(_inst.rectangles()[i],
                          _x_coords[i],
                          _y_coords[i],
                          _inst.rectangles()[j],
                          _x_coords[j],
                          _y_coords[j]))
            {
                return FAIL;
            }
        }
    }
    return SUCCESS;
}

Result Floorplan::print_floorplan() {
    cout << _width << " " << _height << endl;
    for (size_t i = 0; i < _n; i++) {
        cout << _x_coords[i] << " " << _y_coords[i] << endl;
    }
    return SUCCESS;
}

Result Floorplan::print_floorplan_with_dimensions() {
    cout << _width << " " << _height << endl;
    for (size_t i = 0; i < _n; i++) {
        cout << _x_coords[i] << " " << _y_coords[i] 
             << " " << _inst.rectangles()[i].width() << " "
             << _inst.rectangles()[i].height() << endl;
    }
    return SUCCESS;
}


Solver::Solver(const Instance &inst) : _inst(inst), 
                                       _n(inst.n_rectangles())
{
    if (inst.verify() == FAIL)
        cout << "Solver: Warning: unverified instance" << endl;

    set_rectangle_widths();
    set_rectangle_heights();

    _seq_pair = new SequencePair(_n);
    
    /* Make a vertical and a horizontal constraint graph based on
     * the sequence pair. */
    _seq_pair_dag_vert = new SequencePairDAG(_n,
                                             VERTICAL_DU, 
                                            _rectangle_heights,
                                            _seq_pair);
    _seq_pair_dag_hori = new SequencePairDAG(_n,
                                             HORIZONTAL_LR, 
                                            _rectangle_widths,
                                            _seq_pair);
}

Solver::~Solver() {
    delete _seq_pair_dag_hori;
    delete _seq_pair_dag_vert;
    delete _seq_pair;
}

size_t Solver::chip_height() {
    return _seq_pair_dag_vert->longest_path_length();
}

size_t Solver::chip_width() {
    return _seq_pair_dag_hori->longest_path_length();
}

Result Solver::set_rectangle_widths() {
    for (size_t i = 0; i < _n; i++)
        _rectangle_widths.push_back(_inst.rectangles()[i].width());
    return SUCCESS;
}

Result Solver::set_rectangle_heights() {
    for (size_t i = 0; i < _n; i++)
        _rectangle_heights.push_back(_inst.rectangles()[i].height());
    return SUCCESS;
}

size_t Solver::get_area_lower_bound() {
    /* A trivial lower bound is given by the sums
     * of the areas of the rectangles. */
    size_t area_sum = 0;
    for (size_t i = 0; i < _n; i++)
        area_sum += _rectangle_widths[i] * _rectangle_heights[i];
    return area_sum;
}

size_t Solver::get_width_lower_bound() {
    /* A trivial lower bound is given by the min
     * of the widths of the rectangles. */
    return *max_element(_rectangle_widths.begin(), 
                        _rectangle_widths.end());
}

size_t Solver::get_height_lower_bound() {
    /* A trivial lower bound is given by the max
     * of the heights of the rectangles. */
    return *max_element(_rectangle_heights.begin(),
                        _rectangle_heights.end());
}

Result Solver::solve_instance(Floorplan *&ret) {
    vector<size_t> best_Y_coords;
    vector<size_t> best_X_coords;
    size_t best_area = 0;
    size_t best_width, best_height;
    size_t iteration_count = 0;

    size_t area_lower_bound = get_area_lower_bound();
    size_t height_lower_bound = get_height_lower_bound();

    /* For each order of the sequence pair, get chip with and
     * height, and compare area to previous best. If an improvement
     * is found, store the X and Y coords of the rectangles. */
    do {
        size_t W = chip_width();

        /* If width * minimum height > best_area found, 
         * we need not evaluate the height. */
        if (W * height_lower_bound > best_area && best_area != 0)
            continue;

        size_t H = chip_height();
        if (W * H < best_area || best_area == 0) {
            best_width = W;
            best_height = H;
            best_area = W * H;
            best_X_coords = _seq_pair_dag_hori->total_weights_in_order();
            best_Y_coords = _seq_pair_dag_vert->total_weights_in_order();
        }
    } while (_seq_pair->increment() != FAIL && 
             iteration_count++ < MAX_ITERATIONS &&
             best_area > area_lower_bound);

    ret = new Floorplan(_inst,
                        best_width,
                        best_height,
                        best_X_coords,
                        best_Y_coords,
                        UPPER_RIGHT);
    return SUCCESS;
}
/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * sequence_pair.cpp
 *
 */

#include "sequence_pair.hpp"

using namespace std;

Result Sequence::set_initial_sequence() {
    for (size_t i = 0; i < _n; i++)
        _sequence.push_back(i);
    return SUCCESS;
}

Result Sequence::set_initial_order_table() {
    if (_order_table_set)
        return FAIL;

    _order_table = (bool**) calloc(_n, sizeof(bool*));
    for (size_t i = 0; i < _n; i++) {
        _order_table[i] = (bool*) calloc(_n, sizeof(bool));
        for (size_t j = 0; j < _n; j++) {
            _order_table[i][j] = (i < j);
        }
    }

    _order_table_set = true;
    return SUCCESS;
}

Result Sequence::update_order_table() {
    if (_order_table_updated)
        return FAIL;

    if (!_order_table_set)
        set_initial_order_table();

    for (size_t i = 0; i < _n; i++) {
        for (size_t j = 0; j < _n; j++) {
            if (i < j)
                _order_table[_sequence[i]][_sequence[j]] = true;
            else
                _order_table[_sequence[i]][_sequence[j]] = false;
        }
    }

    _order_table_updated = true;
    return SUCCESS;
}

Sequence::Sequence(size_t n) : _n(n), _fact_n(Fact(n)) {
    set_initial_sequence();
}

Sequence::~Sequence() {
    if (_order_table_set) {
        for (size_t i = 0; i < _n; i++) {
            free(_order_table[i]);
        }
        free(_order_table);
    }
}

Result Sequence::increment() {
    next_permutation(_sequence.begin(), _sequence.end());

    _order_table_updated = false;
    _order_number = (_order_number + 1) % _fact_n;
    if (_order_number == 0)
        return FAIL;
    else 
        return SUCCESS;
}

bool Sequence::comes_before(const size_t &x, const size_t &y) {
    
    if (x >= _n || y >= _n) {
        cout << "Sequence: called comes_before for x, y > n" << endl;
        exit(1);
    }

    /* Update the order table if needed. */
    if (!_order_table_updated)
        update_order_table();

    return _order_table[x][y];
}

const size_t &Sequence::order_number() const { return _order_number; }
const vector<size_t> &Sequence::sequence() const { return _sequence; }

Result Sequence::print_sequence() const {
    cout << "S(" << _n << ")@" << _order_number << ": (";
    for (size_t i = 0; i < _n; i++) {
        cout << _sequence[i];
        if (i < _n - 1)
            cout << ", ";
    }
    cout << ")" << endl;
   
    return SUCCESS;
}

Result Sequence::print_order_table() {
    if (!_order_table_updated)
        update_order_table();

    for (size_t i = 0; i < _n; i++) {
        for (size_t j = 0; j < _n; j++) {
            cout << _order_table[i][j] << " ";
        }
        cout << endl;
    }
    return SUCCESS;
}







SequencePair::SequencePair(size_t n) : _n(n), 
                                       _fact_n(Fact(n)),
                                       _pos_seq(Sequence(n)),
                                       _neg_seq(Sequence(n))
{}

Result SequencePair::increment() {
    if (_neg_seq.increment() == FAIL) {
        if(_pos_seq.increment() == FAIL) {
            return FAIL;
        }
    }
    return SUCCESS;
}

bool SequencePair::below(const size_t &x, const size_t &y) {
    return (!_neg_seq.comes_before(x, y) && _pos_seq.comes_before(x, y));
}
bool SequencePair::leftof(const size_t &x, const size_t &y) {
    return (_neg_seq.comes_before(x, y) && _pos_seq.comes_before(x, y));
}
bool SequencePair::rightof(const size_t &x, const size_t &y) {
    return (!_neg_seq.comes_before(x, y) && !_pos_seq.comes_before(x, y));
}
bool SequencePair::above(const size_t &x, const size_t &y) {
    return (_neg_seq.comes_before(x, y) && !_pos_seq.comes_before(x, y));
}


const Sequence &SequencePair::neg_seq() const { return _neg_seq; }
const Sequence &SequencePair::pos_seq() const { return _pos_seq; }


Result SequencePair::print_sequence_pair() const {
    cout << "SP(" << _n << "):" << endl; 
    _pos_seq.print_sequence();
    _neg_seq.print_sequence();
    return SUCCESS;
}



    
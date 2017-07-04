/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * sequence_pair.cpp
 *
 * Implementation of functionality described in sequence_pair.hpp 
 */

#include "sequence_pair.hpp"

using namespace std;

/* Return a vector (0, 1, ... , n - 1). */
vector<size_t> OneToN(size_t n) {
    vector<size_t> ret = vector<size_t>(n, 0);
    for (size_t i = 0; i < n; i++)
        ret[i] = i;

    return ret;
}

Result Sequence::reset() {
    for (size_t i = 0; i < _n; i++)
        _sequence[i] = i;
    _order_number = 0;
    return SUCCESS;
}

Result Sequence::set_initial_order_table() {
    if (_order_table_set)
        return FAIL;

    /* Allocate memory. */
    _order_table = (bool**) calloc(_n, sizeof(bool*));
    for (size_t i = 0; i < _n; i++)
        _order_table[i] = (bool*) calloc(_n, sizeof(bool));

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

Result Sequence::set_order(size_t order_number) {

    if (order_number >= _fact_n)
        return FAIL;

    size_t m = _n;
    size_t k = _fact_n;
    size_t index;
    
    vector<size_t> numbers = OneToN(_n);
    _order_number = order_number;

    for (size_t i = 0; i < _n; i++) {
        k /= m;
        index = order_number / k;
        
        _sequence[i] = numbers[index];
        numbers.erase(numbers.begin() + index);
        
        order_number -= index * k;
        m--;
    }

    return SUCCESS;
}

Sequence::Sequence(size_t n) : _n(n), 
                               _sequence(OneToN(n)) 
{
    try {
        _fact_n = Fact(_n);
    } catch (overflow_error e) {
        cout << "Error: failed to initialize sequence." << endl;
        cout << e.what() << endl;
        exit(1);
    }
}

Sequence::~Sequence() {
    if (_order_table_set) {
        /* Free order table. */
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
    
    #ifndef OPTIMIZED_BUILD
    /* Check that x and y are valid inputs. This is called quite often 
     * so we do not include it in optimized builds. */
    if (x >= _n || y >= _n) {
        cout << "Sequence: called comes_before for x, y > n" << endl;
        exit(1);
    }
    #endif

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
                                       _pos_seq(Sequence(n)),
                                       _neg_seq(Sequence(n))
{
    try {
        _fact_n = Fact(_n);
    } catch (overflow_error e) {
        cout << "Error: failed to initialize sequence pair." << endl;
        cout << e.what() << endl;
        exit(1);
    }
}

Result SequencePair::increment() {
    if (_neg_seq.increment() == FAIL) {
        if(_pos_seq.increment() == FAIL) {
            return FAIL;
        }
    }
    return SUCCESS;
}

Result SequencePair::reset() {
    if (_neg_seq.reset() != FAIL && _pos_seq.reset() != FAIL)
        return SUCCESS;
    else
        return FAIL;
}

Result SequencePair::set_orders(size_t pos_order_number, 
                                size_t neg_order_number)
{
    if (_neg_seq.set_order(neg_order_number) != FAIL && 
        _pos_seq.set_order(pos_order_number) != FAIL) 
    {
        return SUCCESS;
    }
    else
        return FAIL;    
}

bool SequencePair::below(const size_t &x, const size_t &y) {
    return (_neg_seq.comes_before(x, y) && !_pos_seq.comes_before(x, y));
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

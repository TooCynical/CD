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

Sequence::Sequence(size_t n) : _n(n), _fact_n(Fact(n)) {
    set_initial_sequence();
}

Result Sequence::increment() {
    next_permutation(_sequence.begin(), _sequence.end());

    _order_number = (_order_number + 1) % _fact_n;
    if (_order_number == 0)
        return FAIL;
    else 
        return SUCCESS;
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

Result SequencePair::print_sequence_pair() const {
    cout << "SP(" << _n << "):" << endl; 
    _pos_seq.print_sequence();
    _neg_seq.print_sequence();
    return SUCCESS;
}



    
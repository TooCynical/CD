/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * sequence_pair.hpp
 *
 */

#ifndef SEQUENCEPAIR_H
#define SEQUENCEPAIR_H

#include <vector>
#include <iostream>
#include <algorithm>

#include "util.hpp"

class Sequence {
private:
    const size_t _n;
    const size_t _fact_n;
    size_t _order_number = 0;

    std::vector<size_t> _sequence;

    Result set_initial_sequence();

public:
    Sequence(size_t n);
    
    Result increment();
    
    const size_t &order_number() const;
    const std::vector<size_t> &sequence() const;

    Result print_sequence() const;

};

class SequencePair {
private:
    const size_t _n;
    const size_t _fact_n;
    
    Sequence _pos_seq;
    Sequence _neg_seq;
public:
    SequencePair(size_t n);
    Result increment();

    Result print_sequence_pair() const;
};

#endif
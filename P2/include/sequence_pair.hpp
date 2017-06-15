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
    const size_t _n;            // Number of elements in sequence.
    const size_t _fact_n;       // Factorial(n).
    size_t _order_number = 0;   // The index of the current order of the sequence,
                                // according to lexicographical ordering on the 
                                // {1, 2, ... , n}.                 

    std::vector<size_t> _sequence;  // The current (ordered) sequence.

    bool **_order_table;                // nxn table where entry at (i, j) is true
                                        // if i comes before j in the sequence.
    bool _order_table_updated = false;  // Is the order table updated (i.e. accurate).
    bool _order_table_set = false;      // Is memory for order table allocated?

    /* Update the order table if required in O(n^2). */
    Result update_order_table();

    /* Allocate memory for order table and set as if order number = 0. */
    Result set_initial_order_table();

    /* Set sequence to {1,2, ... , n} */
    Result set_initial_sequence();

public:
    Sequence(size_t n);
    ~Sequence();

    /* Change the ordering of the sequence to the lexicographically smallest
     * ordering bigger than the current one. If current order is the biggest
     * possible, change to initial ordering instead, and return FAIL. Update 
     * the order_number accordingly. */ 
    Result increment();

    /* Return whether x comes before y in the sequence. */
    bool comes_before(const size_t &x, const size_t &y);
    
    /* Accessors. */
    const size_t &order_number() const;
    const std::vector<size_t> &sequence() const;

    /* IO-functions for testing. */
    Result print_sequence() const;
    Result print_order_table();
};

class SequencePair {
private:
    const size_t _n;        // Number of elements in each of the sequences.
    const size_t _fact_n;   // Fact(n).
    
    Sequence _pos_seq;      // Positive sequence.
    Sequence _neg_seq;      // Negative sequence.
public:
    SequencePair(size_t n);

    /* Increment the negative sequence. If this causes the negative sequence
     * to be reset, increment the positive sequence as well. If this causes
     * the positive sequence to reset, return FAIL. Basically this is just 
     * incrementing a 2-digit number in n!-ary counting. */
    Result increment();

    /* Return whether x is 'below' y in the sequence pair. */
    bool below(const size_t &x, const size_t &y);
    /* Return whether x is 'left of' y in the sequence pair. */
    bool leftof(const size_t &x, const size_t &y);
    /* Return whether x is 'right of' y in the sequence pair. */
    bool rightof(const size_t &x, const size_t &y);
    /* Return whether x is 'above' y in the sequence pair. */
    bool above(const size_t &x, const size_t &y);

    /* Accessors */
    const Sequence &neg_seq() const;
    const Sequence &pos_seq() const;

    /* IO-functions for testing. */
    Result print_sequence_pair() const;
};

#endif
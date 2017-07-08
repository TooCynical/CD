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

/* Sequence is class that, given a nonnegative integer n, represents
 * the sequence (0, 1, ... , n-1) in one of its orders. 
 * The Sequence is able to:
 *      - Alter the current order by:
 *          - Incrementing it.
 *          - Resetting it to (0, 1, ... , n-1)
 *          - Setting it to the i-th order (according to lexicographical ordering
 *            on all orders).
 *      - Return for 0 <= x,y < n whether x < y in the current order. This
 *        requires an O(n^2)-time preprocessing step for a given order, and
 *        can then be evaluated in constant time so long as the order doesn't
 *        change afterwards.
 *      - Keep track of the order number, indicating the index of the current
 *        order in the list of all orders according to lexicopgraphical ordering. 
 *
 * Detailed information on all members can be found below. */
class Sequence {
private:
    const size_t _n;              // Number of elements in sequence.
    uint64_t _fact_n;             // Factorial(n).
    size_t _order_number = 0;     // The index of the current order of the sequence,
                                  // according to lexicographical ordering on the 
                                  // sequence (1, 2, ... , n). Member functions
                                  // changing the order are responsible for keeping
                                  // this accurate.

    std::vector<size_t> _sequence;      // The current (ordered) sequence.

    bool **_order_table;                // nxn table where entry at (i, j) is true
                                        // if i comes before j in the sequence.
    bool _order_table_updated = false;  // Is the order table updated (i.e. accurate).
    bool _order_table_set = false;      // Is memory for order table allocated?

    /* Update the order table in O(n^2). */
    Result update_order_table();

    /* Allocate memory for order table. */
    Result set_initial_order_table();

public:
    Sequence(size_t n);
    ~Sequence();

    /* Change the ordering of the sequence to the lexicographically smallest
     * ordering bigger than the current one. If current order is the biggest
     * possible, change to initial ordering instead, and return FAIL. */
    Result increment();

    /* Set sequence to (0, 1, ... , n-1) */
    Result reset();

    /* Set the order to match the given order number in O(n^2). */
    Result set_order(size_t order_number);

    /* Return whether x comes before y in the sequence using the order table. */
    bool comes_before(const size_t &x, const size_t &y);
    
    /* Accessors. */
    const size_t &order_number() const;
    const std::vector<size_t> &sequence() const;

    /* IO-functions for testing. */
    Result print_sequence() const;
    Result print_order_table();
};

/* SequencePair is class that, given a nonnegative integer n, represents
 * a pair of two Sequences of length n, one of which is refered to as 
 * 'negative' and the other one as 'positive'.
 * The SequencePair is able to:
 *      - Alter the order of its underlying Sequences.
 *      - Return for 0 <= x,y < n whether x ~ y, where ~ referes to
 *        any one of the 'below', 'leftof', 'rightof' and 'above'
 *        relations, as described in the paper by Murata et al.
 *
 *
 * Detailed information on all members can be found below. */
class SequencePair {
private:
    const size_t _n;        // Number of elements in each of the sequences.
    uint64_t _fact_n;       // Fact(n).
    
    Sequence _pos_seq;      // Positive sequence.
    Sequence _neg_seq;      // Negative sequence.
public:
    SequencePair(size_t n);

    /* Increment the negative sequence. If this causes the negative sequence
     * to be reset, increment the positive sequence as well. If this causes
     * the positive sequence to reset, return FAIL. Basically this is just 
     * incrementing a 2-digit number in n!-ary counting. */
    Result increment();

    /* Reset both the negative and positive sequence. */
    Result reset();

    /* Set order number for positive and negative sequence. */
    Result set_orders(size_t pos_order_number, size_t neg_order_number);


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
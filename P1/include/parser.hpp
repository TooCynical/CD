/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * parser.hpp
 *
 *
 * Parser contains functions to parse a correctly formatted input file
 * resulting in an integer n, and an n x 3 array of integers. Parser will
 * check that:
 *      - The input file exists and is formatted as described in the exercise
 *      - The given amount of terminals does not exceed constant MAX_TERMINALS
 *      - The given coordinates lie between 0 and the constant MAX_COORD
 *      - The input file does not contain duplicate lines.
 */

#ifndef PARSER_H
#define PARSER_H

#include "util.hpp"
#include "instance.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <set>

using namespace std;

/* Attempt to parse the file given by its path. If succesful,
 * use the extracted terminal locations to create a new Instance
 * object, and put a reference to it in ret */
Result ParseFile (const char* file_name, Instance*& ret);

/* Check whether the input file contains duplicate lines. */
Result CheckForDuplicateLines(const char* file_name);

/* Attempt to read terminal locations from file. */
Result ParseTerminals (fstream& input_file, int n, int **term_locs);

#endif

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

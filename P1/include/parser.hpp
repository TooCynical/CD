#ifndef PARSER_H
#define PARSER_H

#include "util.hpp"
#include "instance.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

Result ParseFile (const char* file_name, Instance*& inst);
Result ParseTerminals (fstream& input_file, int n, int **term_locs);

#endif

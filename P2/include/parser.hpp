/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * parser.hpp
 *
 * Parsing of files to create instance objects.
 */

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <vector>

#include "util.hpp"
#include "instance.hpp"

/* Read a file and create an Instance object based on its content,
 * or return FAIL if the file is not formatted correctly. */
Result ParseFile(const char *filename, Instance &inst);

#endif
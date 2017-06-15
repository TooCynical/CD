/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * parser.hpp
 *
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

Result ParseFile(const char *filename, Instance &inst);

#endif
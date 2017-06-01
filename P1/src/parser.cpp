/* Lucas Slot (lfh.slot@gmail.com)
 * Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * parser.cpp
 *
 *
 * Implementation of functionality described in parser.hpp.
 */

#include "parser.hpp"

using namespace std;

Result ParseFile (const char* file_name, Instance*& ret) {
    int n;

    /* Make sure there are no duplicate lines in the file. */
    if (CheckForDuplicateLines(file_name) == FAIL) {
        cout << "ParseFile: Input file contains duplicate lines.\n";
        return FAIL;
    }

    /* Try to open file. */
    fstream input_file(file_name);
    if (!input_file) {
        cout << "ParseFile: File not found: " << file_name << "\n";
        return FAIL;
    }

    /* Try to read number of terminals. */
    input_file >> n;
    if (input_file.fail()) {
        cout << "ParseFile: File format incorrect (no valid n given): " <<
                file_name << "\n";
        return FAIL;
    }

    /* Make sure the number of terminals does not exceed MAX_TERMINALS. */
    if (n > MAX_TERMINALS) {
        cout << "ParseFile: Number of terminals (" << n << ") exceeds "    <<
                "maximum number of terminals allowed (" << MAX_TERMINALS   <<
                ").\nIn order to run bigger instances, please modify the " <<
                "constant MAX_TERMINALS in util.hpp and make sure that "   <<
                "the constant BITSET_SIZE is at least MAX_TERMINALS.\n"    <<
                "Note that runtime might be very high for a large number " << 
                "of terminals!\n";
        return FAIL;
    }

    /* Parse terminal locations. */
    int **term_locs = (int**) calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++)
        term_locs[i] = (int*) calloc(3, sizeof(int));
    
    if (ParseTerminals(input_file, n, term_locs) == FAIL) {
        cout << "ParseFile: Failed to parse terminal locations.\n";
        return FAIL;
    }

    ret = new Instance(n, term_locs);

    Free2DArray(term_locs, n);
    return SUCCESS;
}

Result CheckForDuplicateLines(const char* file_name) {
    fstream input_file(file_name);
    string line;
    set<string> lines;

    /* Read each line, at it to lines and make sure
     * that no duplicates were added. */
    while (getline(input_file, line)) {        
        if (!lines.insert(line).second)
            return FAIL;
    }
    return SUCCESS;
}

Result ParseTerminals (fstream& input_file, int n, int **term_locs) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j ++) {
            if (input_file.fail()) {
                return FAIL;
            }
            input_file >> term_locs[i][j];

            /* Check if terminal coordinates are within range */
            if (term_locs[i][j] < 0 || term_locs[i][j] > MAX_COORD) {
                cout << "The coordinates of a given terminal exceed bounds "   <<
                        "(0-999).\nTo allow bigger coordinates please modify " <<
                        "MAX_COORD in util.hpp.\nNote that correct behaviour " <<
                        "is only guaranteed for coordinates within 0-999.\n";
                return FAIL;
            }

            /* Sometimes we want to force even coordinates. */
            if (DOUBLE_INPUT_COORDS)
                term_locs[i][j] *= 2;
        }
    }
    return SUCCESS;
}
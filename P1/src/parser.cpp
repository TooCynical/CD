#include "parser.hpp"

using namespace std;

Result ParseFile (const char* file_name, Instance*& inst) {
    int n;

    /* Try to open file. */
    fstream input_file(file_name);
    if (!input_file) {
        cout << "File not found: " << file_name << "\n";
        return FAIL;
    }

    /* Try to read number of terminals. */
    input_file >> n;
    if (input_file.fail()) {
        cout << "File format incorrect (no valid n given): " << file_name << "\n";
        return FAIL;
    }

    /* Parse terminal locations. */
    int **term_locs = (int**) calloc(n, sizeof(int*));
    for (int i = 0; i < n; i++)
        term_locs[i] = (int*) calloc(3, sizeof(int));
    if (!ParseTerminals(input_file, n, term_locs)) {
        cout << "Failed to parse terminal locations.\n";
        return FAIL;
    }

    inst = new Instance(n, term_locs);

    Free2DArray(term_locs, n);
    return SUCCESS;
}

Result ParseTerminals (fstream& input_file, int n, int **term_locs) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j ++) {
            if (input_file.fail()) {
                return FAIL;
            }
            input_file >> term_locs[i][j];
        }
    }
    return SUCCESS;
}
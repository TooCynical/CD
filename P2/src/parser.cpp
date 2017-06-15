/*  Lucas Slot (lfh.slot@gmail.com)
 *  Ardalan Khazraei (ardalan.khazraei@gmail.com)
 *
 * June 2017
 * University of Bonn 
 *
 * parser.cpp
 *
 */

#include "parser.hpp"

using namespace std;

Result ReadRectangleDimensions(fstream &input_file, 
                               unsigned int **rectangle_dims,
                               size_t n)
{
    string line;
    unsigned int width, height;
    /* Skip first line. */
    getline(input_file, line);

    /* Read n lines and fetch two unsigned ints from each of them. */
    for (size_t i = 0; i < n; i++) {
        getline(input_file, line);
        istringstream line_stream(line);
        if (line_stream >> width >> height) {
            rectangle_dims[i][0] = width;
            rectangle_dims[i][1] = height;
        }
        else    
            return FAIL;
    }
    return SUCCESS;
}

Result ParseFile(const char *file_name, Instance &inst) {
    size_t n;

    /* Try to open file. */
    fstream input_file(file_name);
    if (!input_file) {
        cout << "ParseFile: File not found: " << file_name << endl;
        return FAIL;
    }

    /* Try to read number of rectangles. */
    input_file >> n;
    if (input_file.fail() || n < 1 || n > MAX_RECTANGLES) {
        cout << "ParseFile: File format incorrect (no valid n given): " <<
                file_name << endl;
        return FAIL;
    }

    /* Try to read rectangles dims line by line. */
    unsigned int **rectangle_dims;
    rectangle_dims = (unsigned int **) calloc(n, sizeof(unsigned int*));
    for (size_t i = 0; i < n; i++)
        rectangle_dims[i] = (unsigned int *) calloc(2, sizeof(unsigned int));

    if (ReadRectangleDimensions(input_file, rectangle_dims, n) == FAIL) {
        cout << "ParseFile: Failed to read rectangle dimensions" << endl;
        return FAIL;
    }

    cout << n << endl;
    inst.set_rectangles(n, rectangle_dims);

    Free2DArray(rectangle_dims, n);
    return SUCCESS;
}

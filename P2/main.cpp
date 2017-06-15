#include "rectangle.hpp"
#include "instance.hpp"
#include "util.hpp"
#include "parser.hpp"

#include <iostream>

using namespace std;

int main(int argc, const char **argv) {

    const char *file_name;
    if (argc > 1)
        file_name = argv[1];
    else
        file_name = "testing/cases/small_test_1";

    Instance inst;
    if (ParseFile(file_name, inst) == FAIL) {
        cout << "Main: Failed to parse file" << endl;
        exit(1);
    }
    if (inst.verify() == FAIL) {
        cout << "Main: Failed to verify instance" << endl;
        exit(1);
    }

    inst.print_rectangles();
    return 0;
}
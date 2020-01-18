/* Implementation of Error Handling Header */

// C Headers
#include <unistd.h>

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "misc/ehandling.hpp"
#include "cli/style.hpp"

using namespace std;

void print_status(const string &msg, bool fatal) {

    cout << "MDX: " << msg << endl;

    if(fatal == true)
        exit(EXIT_FAILURE);
}



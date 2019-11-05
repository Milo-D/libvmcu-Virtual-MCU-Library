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
#include "ehandling.hpp"
#include "style.hpp"

using namespace std;

void print_status(string msg, bool fatal) {

    cout << "MDX: " << msg << endl;

    if(fatal == true)
        exit(EXIT_FAILURE);
}

void print_event(string msg) {

    string select = "";

    do { 

        clrscr();
        cout << "MDX: " << msg << "\n\n";
        cout << "<Press 'c' to continue>\n\n";
        cout << PROMPT;

        getline(cin, select);

    } while(select != "c");
}



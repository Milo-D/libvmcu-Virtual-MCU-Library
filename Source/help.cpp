/* Help Menu Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "help.hpp"
#include "menus.hpp"

using namespace std;

void help(void) {

    string select = "";

    do {

        help_menu();
        getline(cin, select);
        
    } while(select != "c");
}
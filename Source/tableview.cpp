/* Implementation of Table Input/Output Functions */

// C Headers
#include <unistd.h>

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "tableview.hpp"
#include "menus.hpp"
#include "ehandling.hpp"
#include "uparse.hpp"
#include "table.hpp"

using namespace std;

void load_table(Table *table) {

    /* Loading table and setting breakpoints */

    string select;

    do {

        vector <string> cmd;

        table_menu(table);
        getline(cin, select);

        parse_table_in(select).swap(cmd);

        if(cmd.size() == 0) // invalid option
            continue;

        if(cmd[0] == "break")
            table->set_break(cmd[1]);

        if(cmd[0] == "unbreak")
            table->unset_break(cmd[1]);
		
    } while(select != "exit" && select != "e");

}

void show_breaks(void) {

    /* Showing a list of all breakpoints */
}

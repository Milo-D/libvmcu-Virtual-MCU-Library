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
#include "stringmanip.hpp"
#include "parser.hpp"
#include "table.hpp"
#include "menus.hpp"

using namespace std;

void load_table(Table *table) {

    Parser parser(TABLE_CONTEXT);
    string last_select, select;

    do {

        table_menu(table);
        getline(cin, select);

        if(select != "")
            last_select = select;

        vector <string> cmd;
        split(last_select, cmd).swap(cmd);

        switch(parser.parseln(last_select)) {

            case 0: table->set_break(cmd[1]); break;
            case 1: table->unset_break(cmd[1]); break;
            case 2: table->define(cmd[1], cmd[2]); break;
            case 3: table->next_page(+1); break;
            case 4: table->next_page(-1); break;
            case 5: /* leaving... */ break;

            default: /* ignoring invalid input */ break;
        }
		
    } while(select != "e");

}

void show_breaks(void) {

    /* Showing a list of all breakpoints */
}

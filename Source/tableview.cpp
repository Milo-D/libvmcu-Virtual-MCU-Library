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
#include "uparse.hpp"
#include "table.hpp"
#include "menus.hpp"

using namespace std;

void load_table(Table *table) {

    vector <string> cmd;
    string last_select, select;

    do {

        table_menu(table);
        getline(cin, select);

        if(select != "")
            last_select = select;

        if(last_select == "pn")
            table->next_page(+1);

        if(last_select == "pp")
            table->next_page(-1);

        parse_table_in(last_select).swap(cmd);

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

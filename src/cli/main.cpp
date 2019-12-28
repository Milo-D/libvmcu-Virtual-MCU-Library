/* Entry Point */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "cli/mode.hpp"
#include "cli/debugview.hpp"
#include "cli/tableview.hpp"
#include "cli/menus.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"
#include "misc/filemanip.hpp"
#include "parser/parser.hpp"
#include "table/table.hpp"

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;

int main(int argc, char **argv) {

    vector <string> debug_file;

    if(call_mode(argc, argv) == 0)
        return EXIT_SUCCESS;

    get_files(argc, argv).swap(debug_file);

    int fcursor = 0;
    unsigned int file_no = debug_file.size();

    Table *table = create_table(debug_file, file_no);

    Parser parser(MAIN_CONTEXT);
    string select;

    do {

        main_menu(debug_file[fcursor]);
        getline(cin, select);

        switch(parser.parseln(select)) {

            case 0: movec(&fcursor, +1, file_no); break;
            case 1: movec(&fcursor, -1, file_no); break;
		
            case 2: debug(&table[fcursor]); break;
            case 3: load_table(&table[fcursor]); break;
            case 4: show_breaks(); break;
            case 5: cout << "Leaving...\n"; break;

            default: /* ignore invalid input */ break;
        }
		
    } while(select != "e");

    // toDo: cleanup table memory

    return EXIT_SUCCESS;
}

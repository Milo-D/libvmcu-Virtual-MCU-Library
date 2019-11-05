/* Entry Point */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "uparse.hpp"
#include "debugger.hpp"
#include "table.hpp"
#include "tableview.hpp"
#include "menus.hpp"

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;

int main(int argc, char **argv) {			

    vector <string> debug_file;
    parse_main_in(argc, argv).swap(debug_file);

    unsigned int file_no = debug_file.size();

    Table *table = create_table(debug_file, file_no);
	
    char select;
    int fcursor = 0;

    do {

        main_menu(debug_file[fcursor]);
        cin >> select;

        switch(select) {

            case 'p': movec(&fcursor, -1, file_no); break;
            case 'n': movec(&fcursor, +1, file_no); break;
		
            case '0': debug(&table[fcursor]); break;
            case '1': load_table(&table[fcursor]); break;
            case '2': show_breaks(); break;
            case 'e': cout << "Leaving...\n"; break;

            default: /* ignore invalid input */ break;
        }
		
    } while(select != 'e');

    // toDo: cleanup table memory

    return EXIT_SUCCESS;
}


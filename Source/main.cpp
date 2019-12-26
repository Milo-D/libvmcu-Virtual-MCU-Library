/* Entry Point */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "ehandling.hpp"
#include "stringmanip.hpp"
#include "parser.hpp"
#include "table.hpp"
#include "debugview.hpp"
#include "tableview.hpp"
#include "menus.hpp"

#define movec(cursor, offs, range) *cursor = ((*cursor) + offs) % range

using namespace std;

namespace {
    
    void is_hex(string file) {

        unsigned int pos;

        if((pos = file.find(".hex")) == string::npos)
            print_status("Could not parse file.", true);

        if(pos + 4 != file.size())
            print_status("Could not parse file.", true);

        if(pos == 0)
            print_status("Invalid file name.", true);
    }

    vector <string> parse_args(int argsz, char **argls) {

        vector <string> arguments;

        print_status("Parsing files...", false);

        if(argsz < 2)
            print_status("Missing arguments.", true);

        for(int i = 1; i < argsz; i++) {

            string current = (string) argls[i];

            if(file_exists(current) == false)
                print_status("Specified file does not exist.", true);

            is_hex(current);
            arguments.push_back(current);
        }

        print_status("Done!", false);

        return arguments;
    }
};

int main(int argc, char **argv) {			

    vector <string> debug_file;
    parse_args(argc, argv).swap(debug_file);

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


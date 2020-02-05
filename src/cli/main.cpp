/* Entry Point */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <signal.h>

// Project Headers
#include "cli/mainwindow.hpp"
#include "cli/mode.hpp"
#include "cli/debugview.hpp"
#include "misc/stdmsg.hpp"
#include "misc/ehandling.hpp"
#include "misc/stringmanip.hpp"
#include "misc/filemanip.hpp"
#include "parser/parser.hpp"
#include "table/table.hpp"

using namespace std;

static MainWindow *mwin;

int main(int argc, char **argv) {

    if(call_mode(argc, argv) == 0)
        return EXIT_SUCCESS;

    vector <string> debug_file;
    get_files(argc, argv).swap(debug_file);

    int fcursor = 0; int key, i;
    unsigned int file_no = debug_file.size();

    vector <Table> tables;
    create_table(tables, debug_file);

    if(file_no > 4) {

        print_status(MAX_FILE_REACH, true);
    }

    if(file_no == 1)
        debug(&tables[0]);

    mwin = new MainWindow(debug_file);

    do {

        if(((key = mwin->read_key()) == SELECT)) {

            mwin->destroy();

            i = mwin->get_choice();
            debug(&tables[i]);
            mwin->re_init();
        }

    } while(key != QUIT);

    delete mwin;

    return EXIT_SUCCESS;
}

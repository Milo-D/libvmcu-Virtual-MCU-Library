/* MDX Mode Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <chrono>

// Project Headers
#include "cli/mode.hpp"
#include "misc/ehandling.hpp"
#include "misc/filemanip.hpp"
#include "misc/jsonwriter.hpp"
#include "disassembler/disassembler.hpp"
#include "table/table.hpp"
#include "system/sys.hpp"
#include "parser/parser.hpp"

#define time_now() steady_clock::now()

using namespace std;
using namespace std::chrono;

namespace {

    vector <string> check_args(int argsz, char **argls) {

        vector <string> arguments;

        print_status("Parsing files...", false);

        if(argsz < 2)
            print_status("Missing arguments.", true);

        for(int i = 1; i < argsz; i++) {

            string current = (string) argls[i];

            if(file_exists(current) == false)
                print_status("Specified file does not exist.", true);

            is_hex_file(current);
            arguments.push_back(current);
        }

        print_status("Done!", false);

        return arguments;
    }
};

static void mode_disassembler(char *hex_file) {

    vector < tuple <string, int> > origin;
    origin = create_source((string) hex_file);

    for(int i = 0; i < origin.size(); i++)
        cout << get <0> (origin[i]) << endl;
}

static void mode_headless(char *hex_file) {

    Table table((string) hex_file);

    if(table.size() <= 0)
        return;

    Sys sys(&table);

    steady_clock::time_point start = time_now();

    do {

        if(time_now() - start > seconds(5))
            break;

        sys.step();

    } while(sys.is_terminated() == false);

    cout << create_json(&sys);
}

static void mode_help(void) {

    cout << "\nNote that, MDX is still in development.\n";

    cout << "\nARGUMENTS\n";
    cout << " <blank>       <HEX-FILE, ...>           - Interactive Assembly Debugger\n";
    cout << " -d            <HEX-FILE>                - Disassembler\n";
    cout << " -hl           <HEX-FILE>                - Headless Mode\n";
    cout << " -h            <blank>                   - Help\n";

    cout << "\nDEBUG COMMANDS\n";
    cout << " n             <blank>                   - Stepping forward\n";
    cout << " b             <blank>                   - Stepping backwards\n";
    cout << " rn            <blank>                   - Show next GPR Page\n";
    cout << " rp            <blank>                   - Show prev. GPR Page\n";
    cout << " dn            <blank>                   - Next Data memory cell\n";
    cout << " dp            <blank>                   - Prev. Data memory cell\n";
    cout << " jb            <delay in ms>             - Jump to next Breakpoint\n";
    cout << " en            <blank>                   - Next EEPROM memory cell\n";
    cout << " ep            <blank>                   - Prev. EEPROM memory cell\n";
    cout << " q             <blank>                   - Leave / Open File Selector\n";
    cout << " ?             <blank>                   - Show commands (coming soon)\n";
    cout << " break         <line>                    - Set a Breakpoint at 'line'\n";
    cout << " unbreak       <line>                    - Remove Breakpoint at 'line'\n";
    cout << " def           <alias> <seq>             - Redefine Symbols in Table\n";
    cout << " pn            <blank>                   - Show next Source Code Page\n";
    cout << " pp            <blank>                   - Show prev. Source Code Page\n";
    cout << " xd            <address>                 - Examine Data Memory at 'address'\n";
    cout << " xe            <address>                 - Examine EEPROM Memory at 'address'\n";
    cout << " xec           <addr> <range>            - Examine EEPROM Memory as char literal\n";
    cout << " leep          <eep.hex>                 - Load an .eep.hex file into the EEPROM\n";

    cout << "\nGITHUB\n";
    cout << " https://github.com/Milo-D/MDX-Assembly-Debugger/\n";

    cout << "\nAUTHOR OF MDX\n";
    cout << " Name: David Milosevic\n";
    cout << " Email: David.Milosevic@web.de\n";
    cout << " GitHub: Milo-D\n";

    cout << "\nREPORTING BUGS\n";
    cout << " https://github.com/Milo-D/MDX-Assembly-Debugger/issues/\n\n";
}

int call_mode(int argc, char **argv) {

    if(argc < 2)
        print_status("Missing arguments.", true);

    if(check_files(argc, argv) == true)
        return -1;

    string argument;

    for(int i = 1; i < argc; i++) {

        argument += (string) argv[i];

        if(i < argc - 1)
            argument += " ";
    }

    Parser parser(ARG_CONTEXT);
    
    switch(parser.parseln(argument)) {

        case 0: mode_disassembler(argv[2]); break;
        case 1: mode_headless(argv[2]); break;
        case 2: mode_help(); break;

        default:

            string err_str = "Unknown option -> " + argument;
            print_status(err_str, true); 

        break;
    }

    return 0;
}

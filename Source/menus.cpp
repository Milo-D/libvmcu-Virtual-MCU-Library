/* Implementation of all Menus */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "menus.hpp"
#include "mcu.hpp"
#include "table.hpp"
#include "sys.hpp"
#include "style.hpp"

using namespace std;

void main_menu(string current_f) {

    clrscr();
	
    cout << "MDX - AVR Assembly Debugger\n\n";
		
    cout << "Selected File: " << current_f << "\n";
    cout << "prev: <p> next: <n>\n\n";
	
    cout << "< d > Debug " << current_f << "\n";
    cout << "< t > Load Table of " << current_f << "\n";
    cout << "< b > Show Breakpoints of " << current_f << "\n";
    cout << "< e > Exit.\n\n";
	
    cout << PROMPT;
}

void table_menu(Table *table) {

    clrscr();
	
    cout << "MDX - AVR Assembly Debugger\n\n";
    cout << "Set Breaks:  break <address>\n";
    cout << "Undo Breaks: unbreak <address>\n";
    cout << "Exit:        (e)xit\n";
    cout << "\n";

    cout << table->to_str();

    cout << PROMPT;
}

void debug_menu(Sys *sys, int cursor) {

    clrscr();

    cout << "MDX - AVR Assembly Debugger\n\n";
    cout << "Single Step: (n)ext\n";
    cout << "Back Step:   (b)ack\n";
    cout << "Exit:        (e)xit\n";
    cout << "Help:        (?)   \n";
    cout << "\n";

    cout << "Status: ";

    if(sys->is_terminated() == false)
        cout << GREEN << "Running\n";
    else
        cout << RED << "Terminated\n";

    cout << DEFAULT;

    cout << sys->gpr_to_str(cursor);
    cout << sys->sreg_to_str();	
    cout << sys->table_to_str();
    cout << sys->memory_to_str();

    cout << SEPERATOR;
    cout << PROMPT;
}

void help_menu(void) {

    clrscr();

    cout << "MDX - AVR Assembly Debugger\n\n";
    cout << "[Options]     [Arguments]               [Description]\n";
    cout << " n             <none>                    - Single forward Step\n";
    cout << " b             <none>                    - Single backward Step\n";
    cout << " jb            <none>                    - Jumping to next breakpoint or end\n";
    cout << " rn            <none>                    - Show next GPR File Site\n";
    cout << " rp            <none>                    - Show previous GPR File Site\n";
    cout << " dn            <none, int>               - Scroll to next Data Memory Cell\n";
    cout << " dp            <none, int>               - Scroll to previous Data Memory Cell\n";
    cout << " en            <none, int>               - Scroll to next EEPROM Memory Cell\n";
    cout << " ep            <none, int>               - Scroll to previous EEPROM Memory Cell\n";
    cout << " ?             <none>                    - Showing Help Menu\n";
    cout << " e             <none>                    - Exit current Debug Session\n\n";

    cout << "<Press 'c' to continue>\n\n";
    cout << PROMPT;
}


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
	
    cout << "0: Debug " << current_f << "\n";
    cout << "1: Load Table of " << current_f << "\n";
    cout << "2: Show Breakpoints of " << current_f << "\n";
    cout << "e: Exit.\n\n";
	
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

void debug_menu(Sys *sys, Table *table, int cursor) {

    clrscr();

    cout << "MDX - AVR Assembly Debugger\n\n";
    cout << "Single Step: (n)ext\n";
    cout << "Back Step:   (b)ack\n";
    cout << "Exit:        (e)xit\n";
    cout << "\n";

    cout << "Status: ";

    if(sys->is_terminated() == false)
        cout << GREEN << "Running\n";
    else
        cout << RED << "Terminated\n";

    cout << DEFAULT;

    cout << sys->gpr_to_str(cursor);
    cout << sys->sreg_to_str();	
    cout << table->center_to_str();

    cout << PROMPT;
}


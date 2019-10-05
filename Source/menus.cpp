/* Implementation of all Menus */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "menus.hpp"
#include "ehandling.hpp"
#include "table.hpp"
#include "simsys.hpp"

using namespace std;

void main_menu(string current_f) {

	clrscr();
	
	cout << "MDX - Assembly Debugger\n\n";
		
	cout << "Selected File: " << current_f << "\n";
	cout << "prev: <p> next: <n>\n\n";
	
	cout << "0: Debug " << current_f << "\n";
	cout << "1: Load Table of " << current_f << "\n";
	cout << "2: Show Breakpoints of " << current_f << "\n";
	cout << "e: Exit.\n";
}

void table_menu(void) {

	clrscr();
	
	cout << "MDX: Table View\n\n";
	cout << "Set Breaks:  break <address>\n";
	cout << "Undo Breaks: unbreak <address>\n";
	cout << "Exit:        (e)xit\n";
	cout << "\n";
}

void debug_menu(SimSys *sys, Table *table, int seg, int row) {

	clrscr();

	cout << "MDX: Assembly Debugger\n\n";
	cout << "Single Step: (n)ext\n";
	cout << "Back Step:   (b)ack\n";
	cout << "Exit:        (e)xit\n";
	cout << "\n";

	if(sys->terminated == false)
		cout << "Status: Running\n";
	else
		cout << "Status: Terminated\n";
			
	sys->print_reg(seg);
	sys->print_flag();
	table->print_short(row);
	sys->print_stack(row);
}

void prog_int_menu(string msg) {

	clrscr();

	cout << "MDX: " << msg << "\n\n";
	cout << "<Press 'c' to continue>";
	cout << "\n\n";
}

string prog_stdin_menu(void) {

	string read = "";

	clrscr();

	cout << "MDX: Reading from Stdin...\n\n";
	cout << ">>> ";
	cin >> read;

	return read;
}


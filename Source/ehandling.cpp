/* Implementation of Error Handling Header */

// C Headers
#include <unistd.h>

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

// Project Headers
#include "ehandling.hpp"

using namespace std;

void print_status(string msg, bool fatal) {

	cout << "mdx: " << msg << endl;

	if(fatal == true)
		exit(EXIT_FAILURE);
}

void print_delay(string msg, int delay, bool fatal) {

	clrscr();
	cout << "mdx: " << msg << endl;

	sleep(delay);

	/*
	* Switch to cin >>
	**/

	if(fatal == true)
		exit(EXIT_FAILURE);
}

bool file_exists(const string &file) {

	ifstream check_file(file);
	return check_file.good();
}



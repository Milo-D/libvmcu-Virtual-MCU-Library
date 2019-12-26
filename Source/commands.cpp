/* MDX Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "commands.hpp"

using namespace std;

int cmd_count[CONTEXT] = { 6, 5, 9 };

string commands[CONTEXT][12] = {
	
	{ "n", "p", "d", "t", "b", "e" },                              /* Main Commands */
	{ "break", "unbreak", "pn", "pp", "e" },                       /* Table Commands */
	{ "n", "rn", "dn", "dp", "jb", "en", "ep", "e", "?" }          /* Debug Commands */
	
};

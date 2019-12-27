/* MDX Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "commands.hpp"

using namespace std;

int cmd_count[CONTEXT] = { 3, 6, 6, 9 };

string commands[CONTEXT][12] = {
	
    { "-d", "-hl", "-h" },                                         /* CL Arguments */
	{ "n", "p", "d", "t", "b", "e" },                              /* Main Commands */
	{ "break", "unbreak", "def", "pn", "pp", "e" },                /* Table Commands */
	{ "n", "rn", "dn", "dp", "jb", "en", "ep", "e", "?" }          /* Debug Commands */
	
};

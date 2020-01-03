/* MDX Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "parser/commands.hpp"

using namespace std;

int cmd_count[CONTEXT] = { 3, 6, 6, 11 };

string commands[CONTEXT][12] = {
	
    { "-d", "-hl", "-h" },                                                 /* CL Arguments */
	{ "n", "p", "d", "t", "b", "e" },                                      /* Main Commands */
	{ "break", "unbreak", "def", "pn", "pp", "e" },                        /* Table Commands */
	{ "n", "b", "rn", "rp", "dn", "dp", "jb", "en", "ep", "e", "?" }       /* Debug Commands */
	
};

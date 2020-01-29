/* MDX Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "parser/commands.hpp"

using namespace std;

int cmd_count[CONTEXT] = { 3, MAX_COM };

string commands[CONTEXT][MAX_COM] = {
	
    { "-d", "-hl", "-h" },                                                             /* CL Arguments */
	{ "n", "b", "rn", "rp", "dn", "dp", "jb", "en", "ep", "xd", "xe",                  /* Debug Commands */
      "xec", "leep", "q", "?", "break", "unbreak", "def", "pn", "pp" }
	
};

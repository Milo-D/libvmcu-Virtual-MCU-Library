/* MDX Commands Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "parser/commands.hpp"

using namespace std;

int cmd_count[CONTEXT] = { 3, 17 };

string commands[CONTEXT][17] = {
	
    { "-d", "-hl", "-h" },                                                      /* CL Arguments */
	{ "n", "b", "rn", "rp", "dn", "dp", "jb", "en", "ep", "xd", "q", "?",       /* Debug Commands */
      "break", "unbreak", "def", "pn", "pp" }
	
};

/* Commands Header */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "parser/commands.h"

const int ncmd[2] = { 3, NCOM };

const char *commands[NCON][NCOM] = {
    
    /* ARG CONTEXT */
    { "-d", "-hl", "-h" },

    /* DEBUG CONTEXT */
    { "n", "b", "rn", "rp", "dn", "dp", "jb", "en", "ep", "xd", "xe", "xdc",
      "xec", "leep", "clear", "q", "?", "break", "unbreak", "def", "pn",               
      "pp", "cycles" }                                                      
};
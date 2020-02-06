/* Parser Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "parser/parser.hpp"
#include "parser/commands.hpp"
#include "misc/ehandling.hpp"
#include "misc/filemanip.hpp"
#include "misc/stringmanip.hpp"

using namespace std;

/* --- Public --- */

Parser::Parser(int context) {
	
    for(int i = 0; i < cmd_count[context]; i++)
        this->cmap[ commands[context][i] ] = i;
	
    this->context = context;
}

int Parser::parseln(string cmd) {
	
    vector <string> cmd_split;
    split(cmd, cmd_split).swap(cmd_split);
	
    map <string, int> :: iterator it;

    if((it = this->cmap.find(cmd_split[0])) == this->cmap.end())
        return -1;

    bool passed = false;
    
    switch(this->context) {

        case ARG_CONTEXT: passed = this->parse_arg(cmd_split); break;
        case DEBUG_CONTEXT: passed = this->parse_debug(cmd_split); break;

        default: break;
    }

    return (passed ? this->cmap[cmd_split[0]] : -1);
}

/* --- Private --- */

bool Parser::parse_arg(vector <string> cmd) {

    int index = this->cmap[cmd[0]];
    int argc = cmd.size() - 1;

    switch(index) {

        case 0: case 1:

            if(argc != 1)
                print_status("Missing File.", true);

            if(file_exists(cmd[1]) == false)
                print_status("File does not exist.", true);

        break;

        case 2:

            if(argc != 0)
                print_status("Too many arguments.", true);

        break;

        default: return false;
    }

    return true;
}

bool Parser::parse_debug(vector <string> cmd) {
	
    int index = this->cmap[cmd[0]];
    int argc = cmd.size() - 1;

    switch(index) {

        case 0: case 1: case 2: case 3: case 4:

            if(argc != 0)
                return false;

        break;

        case 5:

            if(argc != 0)
                return false;

        break;

        case 6:

            if(argc != 1)
                return false;

        break;

        case 7: case 8:

            if(argc != 0)
                return false;

        break;
        
        case 9: case 10:

            if(argc != 1)
                return false;

        break;

        case 11:

            if(argc != 2)
                return false;

        break;

        case 12:

            if(argc != 1)
                return false;

        break;

        case 13: case 14: case 15:

            if(argc != 0)
                return false;

        break;

        case 16: case 17:
        
            if(argc != 1 || to_dec(cmd[1]) < 0)
                return false;
        
        break;

        case 18:

            if(argc != 2)
                return false;

        break;
        
        case 19: case 20:
        
            if(argc != 0)
                return false;
        
        break;
        
        default: print_status("Could not parse line.", true); break;
    }
		
    return true;
}

/* Parser Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "parser.hpp"
#include "commands.hpp"
#include "ehandling.hpp"
#include "stringmanip.hpp"

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
    
    if(this->context == MAIN_CONTEXT)
        passed = this->parse_main(cmd_split);
		
    if(this->context == TABLE_CONTEXT)
        passed = this->parse_table(cmd_split);
		
    if(this->context == DEBUG_CONTEXT)
        passed = this->parse_debug(cmd_split);
	
    return (passed ? this->cmap[cmd_split[0]] : -1);
}

/* --- Private --- */

bool Parser::parse_main(vector <string> cmd) {

    int argc = cmd.size() - 1;
    
    if(argc == 0)
        return true;
	
    return false;
}

bool Parser::parse_table(vector <string> cmd) {
	
    int index = this->cmap[cmd[0]];
    int argc = cmd.size() - 1;
	
    switch(index) {
		
        case 0: case 1:
		
            if(argc != 1 || to_dec(cmd[1]) < 0)
                return false;
		
        break;
		
        case 2: case 3: case 4:
		
            if(argc != 0)
                return false;
		
        break;
		
        default: print_status("Could not parse line.", true); break;
    }
	
    return true;
}

bool Parser::parse_debug(vector <string> cmd) {
	
    int argc = cmd.size() - 1;
	
    if(argc == 0)
        return true;
		
    return false;
}

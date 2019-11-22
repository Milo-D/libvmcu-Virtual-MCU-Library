/* Implementation of DebugParser */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <iterator>

// Project Headers
#include "debugparser.hpp"

#define MAX 9

using namespace std;

DebugParser::DebugParser(void) {

    string valid[MAX] = { "n", "rn", "dn", "dp", "jb", "en", "ep", "e", "?" };

    for(int i = 0; i < MAX; i++)
        this->cmd[ valid[i] ] = i;
}

int DebugParser::index_of(string input) {

    map <string, int> :: iterator it;

    if((it = this->cmd.find(input)) == this->cmd.end())
        return -1;

    return this->cmd[input];
}
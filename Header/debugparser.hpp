/* Debug Input Parser */

#ifndef DEBUGPARSER_HPP
#define DEBUGPARSER_HPP

#include <map>

class DebugParser {

public:
    DebugParser(void);                              // DebugParser Constructor

    int index_of(std::string input);                // returning index of debug commands

private:
    std::map <std::string, int> cmd;
};

#endif
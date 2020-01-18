/* Parser Header */

#ifndef PARSER_HPP
#define PARSER_HPP

#define ARG_CONTEXT 0
#define DEBUG_CONTEXT 1

// C++ Headers
#include <vector>
#include <map>

class Parser {
	
public:
	Parser(int context);
	
	int parseln(std::string cmd);

private:
	int context;
	std::map <std::string, int> cmap;
	
    bool parse_arg(std::vector <std::string> cmd);
	bool parse_debug(std::vector <std::string> cmd);
};

#endif

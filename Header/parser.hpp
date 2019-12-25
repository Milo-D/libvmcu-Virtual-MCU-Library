/* Parser Header */

#ifndef PARSER_HPP
#define PARSER_HPP

#define MAIN_CONTEXT 0
#define TABLE_CONTEXT 1
#define DEBUG_CONTEXT 2

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
	
	bool parse_main(std::vector <std::string> cmd);
	bool parse_table(std::vector <std::string> cmd);
	bool parse_debug(std::vector <std::string> cmd);
};

#endif

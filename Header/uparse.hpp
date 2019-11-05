/* User Input Parser Header */

#ifndef UPARSE_HPP
#define UPARSE_HPP

#include <vector>

std::vector <std::string> parse_main_in(int argsz, char **argls);
std::vector <std::string> parse_table_in(std::string cmd);

#endif

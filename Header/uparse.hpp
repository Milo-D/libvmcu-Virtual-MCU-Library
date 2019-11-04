/* User Input Parser Header */

#ifndef UPARSE_HPP
#define UPARSE_HPP

#include <vector>

std::vector <std::string> parse_main_in(int argsz, char **argls);
std::vector <std::string> parse_table_in(std::string cmd);
std::string get_file_name(std::string file);
std::string del_comment(std::string line);
void trim(std::string *x);
int to_dec(std::string input);
int hex_to_dec(std::string input);

#endif

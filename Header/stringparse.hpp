/* String-Parse Header */

#ifndef STRINGPARSE_HPP
#define STRINGPARSE_HPP

// C++ Header
#include <vector>

std::string get_file_name(std::string file);
std::string del_comment(std::string line);
void trim(std::string *x);
int to_dec(std::string input);
int hex_to_dec(std::string input);
std::string mix_memory(std::vector <std::string> first, std::vector <std::string> second);
bool file_exists(const std::string &file);

#endif
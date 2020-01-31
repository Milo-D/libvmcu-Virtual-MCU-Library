/* String-Manipulators Header */

#ifndef STRINGMANIP_HPP
#define STRINGMANIP_HPP

// C++ Header
#include <vector>

std::string get_file_name(std::string file);
std::string del_comment(std::string line);
std::string fill(int len);
std::string replace_str(std::string line, const std::string& alias, const std::string& seq);
std::vector <std::string> split(std::string s, std::vector <std::string> res);
void trim(std::string *x);
int to_dec(std::string input);
int hex_to_dec(std::string input);
std::string get_hex(int8_t input);
std::string swap_bytes(const std::string& hex, const int pos);

#endif
/* String-Manipulators Header */

#ifndef STRINGMANIP_HPP
#define STRINGMANIP_HPP

// C++ Header
#include <vector>

std::string get_file_name(std::string file);
std::string del_comment(std::string line);
std::string fill(int len);
void trim(std::string *x);
int to_dec(std::string input);
int hex_to_dec(std::string input);
std::string get_hex(int8_t input);
std::string mix_memory(std::vector <std::string> first, std::vector <std::string> second);
bool file_exists(const std::string &file);

#endif
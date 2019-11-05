/* String-Parse Header */

#ifndef STRINGPARSE_HPP
#define STRINGPARSE_HPP

std::string get_file_name(std::string file);
std::string del_comment(std::string line);
void trim(std::string *x);
int to_dec(std::string input);
int hex_to_dec(std::string input);
bool file_exists(const std::string &file);

#endif
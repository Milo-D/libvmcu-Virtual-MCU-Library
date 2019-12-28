/* File Manipulation Header */

#ifndef FILEMANIP_HPP
#define FILEMANIP_HPP

bool file_exists(const std::string &file);
bool is_hex_file(std::string file);
bool check_files(int argc, char **argv);
std::vector <std::string> get_files(int argc, char **argv);

#endif
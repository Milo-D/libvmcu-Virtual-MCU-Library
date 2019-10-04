/* Error Handling Header */

#ifndef EHANDLING_HPP
#define EHANDLING_HPP

#define clrscr() system("clear")

void print_status(std::string msg, bool fatal);
void print_delay(std::string msg, int delay, bool fatal);
bool file_exists(const std::string &file);

#endif
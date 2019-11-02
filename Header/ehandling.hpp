/* Error Handling Header */

#ifndef EHANDLING_HPP
#define EHANDLING_HPP

#define clrscr() system("clear")

void print_status(std::string msg, bool fatal);
void print_event(std::string msg);
bool file_exists(const std::string &file);

#endif

/* String-Manipulators Header */

#ifndef VMCU_STRINGMANIP_H
#define VMCU_STRINGMANIP_H

extern int strpos(const char *str, const char *sub);
extern char* substr(const char *str, const int from, const int to);
extern int htoi(const char *input);
extern int swap_bytes(char *buffer, const char *bytes, const int pos);

#endif

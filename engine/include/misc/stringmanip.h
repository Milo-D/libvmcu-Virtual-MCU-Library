/* String-Manipulators Header */

#ifndef VMCU_STRINGMANIP_H
#define VMCU_STRINGMANIP_H

extern int vmcu_strpos(const char *str, const char *sub);
extern char* vmcu_substr(const char *str, const int from, const int to);
extern int vmcu_htoi(const char *input);

#endif

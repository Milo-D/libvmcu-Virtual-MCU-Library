/* String-Manipulators Header */

#ifndef VMCU_STRINGMANIP_H
#define VMCU_STRINGMANIP_H

// C Headers
#include <inttypes.h>

extern int vmcu_strpos(const char *str, const char *sub);
extern char* vmcu_substr(const char *str, const int from, const int to);
extern int64_t vmcu_htoi(const char *input);

#endif

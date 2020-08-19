/* String-Manipulators Header */

#ifndef STRINGMANIP_H
#define STRINGMANIP_H

// C Headers
#include <inttypes.h>

typedef struct _list list_t;

extern int strpos(const char *str, const char *sub);
extern char* substr(const char *str, const int from, const int to);
extern char* strxcat(const char *one, const char *two);
extern char* strrepl(const char *str, const char *alias, const char *seq);
extern int fname_of(const char *path, char *buffer);
extern int del_comment(const char *line, char *buffer);
extern char* strfill(const char fill, const int len, const int atl);
extern void split(const char *str, const char at, list_t *buffer);
extern int get_int(const char *str);
extern char* get_str(const int num);
extern int htoi(const char *input);
extern char* itoh(const int num);
extern void to_hex(int8_t input, char *buffer);
extern int swap_bytes(char *buffer, const char *bytes, const int pos);

#endif

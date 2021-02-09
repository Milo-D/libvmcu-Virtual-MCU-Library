/* Prompt Header */

#ifndef PROMPT_H
#define PROMPT_H

#define PLEN 64

// Project Headers (debugger)
#include "debugger/include/cli/util/properties.h"

typedef struct _prompt {

	WINDOW *win;

} prompt_t;

extern struct _prompt* prompt_ctor(void);
extern void prompt_dtor(struct _prompt *this);

extern void prompt_read(const struct _prompt *this, char *buffer);
extern void prompt_write(const struct _prompt *this, const char *str);

extern void prompt_resize(struct _prompt *this);
extern void prompt_update(const struct _prompt *this);

#endif

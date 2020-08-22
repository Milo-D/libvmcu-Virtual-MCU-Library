/* Prompt Header */

#ifndef PROMPT_H
#define PROMPT_H

#define PLEN 64

struct _private;

typedef struct _prompt {

    struct _private *p;

} prompt_t;

extern struct _prompt* prompt_ctor(int h, int w, int y, int x);
extern void prompt_dtor(struct _prompt *this);

extern void prompt_read(const struct _prompt *this, char *buffer);
extern void prompt_write(const struct _prompt *this, const char *str);

extern void prompt_resize(struct _prompt *this, int h, int w, int y, int x);
extern void prompt_update(const struct _prompt *this);

#endif
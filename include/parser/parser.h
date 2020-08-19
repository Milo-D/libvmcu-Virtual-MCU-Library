/* Parser Header */

#ifndef PARSER_H
#define PARSER_H

typedef enum { ARG, DEBUG } CONTEXT;

struct _private;

typedef struct _parser {

    CONTEXT context;
    struct _private *p;

} parser_t;

extern struct _parser* parser_ctor(CONTEXT context);
extern void parser_dtor(struct _parser *this);

extern int parser_eval(const struct _parser *this, const char *cmd);

#endif

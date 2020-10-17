/* Parser Header */

#ifndef PARSER_H
#define PARSER_H

typedef struct _map map_t;
typedef enum { ARG, DEBUG } CONTEXT;

typedef struct _parser {

    map_t *map;
    CONTEXT context;

} parser_t;

extern struct _parser* parser_ctor(CONTEXT context);
extern void parser_dtor(struct _parser *this);

extern int parser_eval(const struct _parser *this, const char *cmd);

#endif

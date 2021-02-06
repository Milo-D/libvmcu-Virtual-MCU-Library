/* String Map Header */

#ifndef STRMAP_H
#define STRMAP_H

struct _private;

typedef struct _strmap {

    int size;
    int capacity;

    struct _private *p;

} strmap_t;

extern struct _strmap* strmap_ctor(const int n);
extern void strmap_dtor(struct _strmap *this);

extern int strmap_put(struct _strmap *this, const char *str);
extern int strmap_get(const struct _strmap *this, const char *str);

#endif
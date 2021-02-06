/* LabelMap Header */

#ifndef LABELMAP_H
#define LABELMAP_H

typedef struct _plain plain_t;
typedef struct _array array_t;
typedef struct _strmap strmap_t;

typedef struct _lmap {

    array_t *labels;
    strmap_t *map;

    int size;

} lmap_t;

extern struct _lmap* lmap_ctor(void);
extern void lmap_dtor(struct _lmap *this);

extern int lmap_add(struct _lmap *this, const char *ln, const int i);
extern int lmap_get(const struct _lmap *this, const int index, plain_t *buffer);

#endif
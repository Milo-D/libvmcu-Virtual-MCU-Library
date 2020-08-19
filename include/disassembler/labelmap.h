/* LabelMap Header */

#ifndef LABELMAP_H
#define LABELMAP_H

struct _private;
typedef struct _tuple tuple_t;

typedef struct _lmap {

    int size;
    struct _private *p;

} lmap_t;

extern struct _lmap* lmap_ctor(void);
extern void lmap_dtor(struct _lmap *this);

extern int lmap_add(struct _lmap *this, const char *ln, const int i);
extern void lmap_get(const struct _lmap *this, const int index, tuple_t *buffer);
extern int lmap_sort(struct _lmap *this, const int l, const int h);

#endif
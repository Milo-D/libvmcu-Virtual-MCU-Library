/* Tuple Header */

#ifndef TUPLE_H
#define TUPLE_H

struct _private;

typedef enum { INT64, INT32, INT16, INT8,
               UINT64, UINT32, UINT16, UINT8,
               BOOL, LONG, FLOAT, INT, STR, VOID } DTYPE;

typedef struct _tuple {

    int size;
    struct _private *p;

} tuple_t;

extern struct _tuple* tuple_ctor(const int n, ...);
extern struct _tuple* tuple_cpy(const struct _tuple *src);
extern void tuple_dtor(struct _tuple *this);

extern int tuple_set(struct _tuple *this, const void *value, const size_t size, const int index);
extern void* tuple_get(const struct _tuple *this, const int index);
extern DTYPE tuple_type(const struct _tuple *this, const int index);

#endif

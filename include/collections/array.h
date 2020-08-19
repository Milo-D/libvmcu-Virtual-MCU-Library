/* Array Header */

#ifndef ARRAY_H
#define ARRAY_H

// C Headers
#include <inttypes.h>

struct _private;

typedef struct _array {

    int top;
    int size;

    struct _private *p;

} array_t;

extern struct _array* array_ctor(const int size, const void *ffp, const void *cfp);
extern void array_dtor(struct _array *this);

extern int array_push(struct _array *this, void *value, const size_t bytes);
extern void** array_pop(struct _array *this);

extern int array_set(struct _array *this, void *value, const size_t bytes, const int index);
extern void array_delete(struct _array *this, const int index);
extern int array_insert(struct _array *this, void *value, const size_t bytes, const int index);
extern void array_swap(struct _array *this, const int i, const int j);

extern void* array_at(const struct _array *this, const int index);

#endif
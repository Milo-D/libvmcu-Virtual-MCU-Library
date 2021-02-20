/* Array Header */

#ifndef VMCU_ARRAY_H
#define VMCU_ARRAY_H

// C Headers
#include <inttypes.h>

struct private;

typedef struct array {

    int top;
    int size;

    struct private *p;

} array_t;

extern array_t* array_ctor(const int size, const void *ffp, const void *cfp);
extern void array_dtor(array_t *this);

extern int array_push(array_t *this, void *value, const size_t bytes);
extern void** array_pop(array_t *this);

extern int array_set(array_t *this, void *value, const size_t bytes, const int index);
extern void array_delete(array_t *this, const int index);
extern int array_insert(array_t *this, void *value, const size_t bytes, const int index);
extern void array_swap(array_t *this, const int i, const int j);

extern void* array_at(const array_t *this, const int index);

#endif
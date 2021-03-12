/* Array Header */

#ifndef VMCU_ARRAY_H
#define VMCU_ARRAY_H

// C Headers
#include <inttypes.h>

typedef struct vmcu_array_private vmcu_array_private_t;

typedef struct vmcu_array {

    int top;
    int size;

    vmcu_array_private_t *p;

} vmcu_array_t;

extern vmcu_array_t* vmcu_array_ctor(const int size, const void *ffp, const void *cfp);
extern void vmcu_array_dtor(vmcu_array_t *this);

extern int vmcu_array_push(vmcu_array_t *this, void *value, const size_t bytes);
extern void** vmcu_array_pop(vmcu_array_t *this);

extern int vmcu_array_set(vmcu_array_t *this, void *value, const size_t bytes, const int index);
extern void vmcu_array_delete(vmcu_array_t *this, const int index);
extern int vmcu_array_insert(vmcu_array_t *this, void *value, const size_t bytes, const int index);
extern void vmcu_array_swap(vmcu_array_t *this, const int i, const int j);

extern void* vmcu_array_at(const vmcu_array_t *this, const int index);

#endif
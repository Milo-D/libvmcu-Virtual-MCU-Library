/* Tuple Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers
#include "collections/tuple.h"

struct _private {

    void **value;
    size_t *bytes;

    DTYPE *type;
};

extern struct _tuple* tuple_ctor(const int n, ...) {

    struct _tuple *tuple;

    if(n < 1) 
        return NULL;

    if((tuple = malloc(sizeof(struct _tuple))) == NULL)
        return NULL;

    if((tuple->p = malloc(sizeof(struct _private))) == NULL) {

        free(tuple);
        return NULL;
    }

    va_list var;
    va_start(var, n);

    tuple->p->type = malloc(n * sizeof(DTYPE));
    tuple->p->value = malloc(n * sizeof(void*));
    tuple->p->bytes = malloc(n * sizeof(size_t));

    for(int i = 0; i < n; i++)
        tuple->p->type[i] = va_arg(var, DTYPE);

    for(int i = 0; i < n; i++) {

        tuple->p->value[i] = NULL;
        tuple->p->bytes[i] = 0;
    }

    tuple->size = n;
    return tuple;
}

extern struct _tuple* tuple_cpy(const struct _tuple *src) {

    struct _tuple *dest;

    if(src == NULL)
        return NULL;

    if((dest = malloc(sizeof(struct _tuple))) == NULL)
        return NULL;

    if((dest->p = malloc(sizeof(struct _private))) == NULL) {

        free(dest);
        return NULL;
    }

    dest->p->type = malloc(src->size * sizeof(DTYPE));
    dest->p->value = malloc(src->size * sizeof(void*));
    dest->p->bytes = malloc(src->size * sizeof(size_t));

    dest->size = src->size;

    for(int i = 0; i < src->size; i++) {
        
        dest->p->type[i] = src->p->type[i];
        dest->p->value[i] = NULL;

        tuple_set(dest, src->p->value[i], src->p->bytes[i], i);
    }

    return dest;
}

extern void tuple_dtor(struct _tuple *this) {

    for(int i = 0; i < this->size; i++) {

        if(this->p->value[i] != NULL)
            free(this->p->value[i]);
    }

    free(this->p->bytes);
    free(this->p->value);
    free(this->p->type);
    free(this->p);
    free(this);
}

extern int tuple_set(struct _tuple *this, const void *value, const size_t size, const int index) {

    if(this == NULL)
        return -1;

    if(index < 0 || index >= this->size)
        return -1;

    if(this->p->value[index] != NULL)
        free(this->p->value[index]);

    this->p->value[index] = malloc(size);
    memcpy(this->p->value[index], value, size);
    this->p->bytes[index] = size;

    return 0;
}

extern void* tuple_get(const struct _tuple *this, const int index) {

    if(index < 0 || index >= this->size)
        return NULL;

    return this->p->value[index];
}

extern DTYPE tuple_type(const struct _tuple *this, const int index) {

    if(index < 0 || index >= this->size)
        return VOID;

    return this->p->type[index];    
}





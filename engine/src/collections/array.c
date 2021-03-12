/* Array Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/collections/array.h"

typedef struct vmcu_array_private {

    int size;
    void **block;

    void (*ffp)(void *fp);
    void* (*cfp)(void *cp);

} vmcu_array_private_t;

/* Forward Declaration of private Functions */

static int set(vmcu_array_t *this, void *e, const size_t bytes, const int index);
static void delete(void **e, void (*ffp)(void *fp));
static inline void sync(vmcu_array_t *this);

/* --- Extern --- */

vmcu_array_t* vmcu_array_ctor(const int size, const void *ffp, const void *cfp) {

    vmcu_array_t *array;

    if((array = malloc(sizeof(vmcu_array_t))) == NULL)
        return NULL;

    if((array->p = malloc(sizeof(vmcu_array_private_t))) == NULL) {

        free(array);
        return NULL;
    }

    array->p->block = malloc(size * sizeof(void*));

    for(int i = 0; i < size; i++)
        array->p->block[i] = NULL;

    array->p->ffp = ffp;
    array->p->cfp = cfp;

    array->p->size = size;
    array->size = size;

    array->top = 0;

    return array;
}

void vmcu_array_dtor(vmcu_array_t *this) {

    sync(this);

    for(int i = 0; i < this->size; i++)  {

        if(this->p->block[i] == NULL)
            continue;

        delete(&this->p->block[i], this->p->ffp);
    }

    free(this->p->block);
    free(this->p);
    free(this);
}

int vmcu_array_push(vmcu_array_t *this, void *value, const size_t bytes) {

    sync(this);

    if(this->p->block[this->top] != NULL)
        delete(&this->p->block[this->top], this->p->ffp);

    return set(this, value, bytes, this->top++);
}

void** vmcu_array_pop(vmcu_array_t *this) {

    sync(this);
    this->top -= 1;

    return &this->p->block[this->top];
}

int vmcu_array_set(vmcu_array_t *this, void *value, const size_t bytes, const int index) {

    sync(this);

    if(this->p->block[index] != NULL)
        delete(&this->p->block[index], this->p->ffp);

    return set(this, value, bytes, index);
}

void vmcu_array_delete(vmcu_array_t *this, const int index) {

    sync(this);
    delete(&this->p->block[index], this->p->ffp);
}

int vmcu_array_insert(vmcu_array_t *this, void *value, const size_t bytes, const int index) {

    sync(this);

    for(int i = this->size - 1; i > index;  i--) {

        if(this->p->block[i] != NULL)
            delete(&this->p->block[i], this->p->ffp);

        if(this->p->block[i - 1] != NULL)
            set(this, this->p->block[i - 1], bytes, i);
        else
            this->p->block[i] = NULL;
    }

    if(this->p->block[index] != NULL)
        delete(&this->p->block[index], this->p->ffp);
    
    return set(this, value, bytes, index);
}

void vmcu_array_swap(vmcu_array_t *this, const int i, const int j) {

    sync(this);

    void *tmp = this->p->block[i];
    this->p->block[i] = this->p->block[j];
    this->p->block[j] = tmp;
}

void* vmcu_array_at(const vmcu_array_t *this, const int index) {

    return this->p->block[index];
}

/* --- Static --- */

static int set(vmcu_array_t *this, void *e, const size_t bytes, const int index) {

    if(this->p->cfp != NULL) {

        this->p->block[index] = (void*) (*this->p->cfp)(e);
        return 0;
    }

    if((this->p->block[index] = malloc(bytes)) < 0)
        return -1;

    memcpy(this->p->block[index], e, bytes);
    return 0;
}

static void delete(void **e, void (*ffp)(void *fp)) {

    if(ffp != NULL)
        (*ffp)(*e);
    else
        free(*e);

    *e = NULL;
}

static inline void sync(vmcu_array_t *this) {

    if(this->size == this->p->size)
        return;

    const size_t bytes = this->size * sizeof(void*);
    this->p->block = realloc(this->p->block, bytes);

    if(this->size < this->p->size) {

        this->p->size = this->size;
        return;
    }

    for(int i = this->p->size; i < this->size; i++)
        this->p->block[i] = NULL;

    this->p->size = this->size;
}

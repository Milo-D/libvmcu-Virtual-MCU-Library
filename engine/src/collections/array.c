/* Array Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/collections/array.h"

struct private {

    int size;
    void **block;

    void (*ffp)(void *fp);
    void* (*cfp)(void *cp);
};

/* Forward Declaration of private Functions */

static int set(array_t *this, void *e, const size_t bytes, const int index);
static void delete(void **e, void (*ffp)(void *fp));
static inline void sync(array_t *this);

/* --- Extern --- */

array_t* array_ctor(const int size, const void *ffp, const void *cfp) {

    array_t *array;

    if((array = malloc(sizeof(array_t))) == NULL)
        return NULL;

    if((array->p = malloc(sizeof(struct private))) == NULL) {

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

void array_dtor(array_t *this) {

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

int array_push(array_t *this, void *value, const size_t bytes) {

    sync(this);

    if(this->p->block[this->top] != NULL)
        delete(&this->p->block[this->top], this->p->ffp);

    return set(this, value, bytes, this->top++);
}

void** array_pop(array_t *this) {

    sync(this);
    this->top -= 1;

    return &this->p->block[this->top];
}

int array_set(array_t *this, void *value, const size_t bytes, const int index) {

    sync(this);

    if(this->p->block[index] != NULL)
        delete(&this->p->block[index], this->p->ffp);

    return set(this, value, bytes, index);
}

void array_delete(array_t *this, const int index) {

    sync(this);
    delete(&this->p->block[index], this->p->ffp);
}

int array_insert(array_t *this, void *value, const size_t bytes, const int index) {

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

void array_swap(array_t *this, const int i, const int j) {

    sync(this);

    void *tmp = this->p->block[i];
    this->p->block[i] = this->p->block[j];
    this->p->block[j] = tmp;
}

void* array_at(const array_t *this, const int index) {

    return this->p->block[index];
}

/* --- Static --- */

static int set(array_t *this, void *e, const size_t bytes, const int index) {

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

static inline void sync(array_t *this) {

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

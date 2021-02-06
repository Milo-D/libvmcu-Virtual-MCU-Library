/* Char Queue Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Project Headers
#include "collections/queue.h"

#define INIT_SIZE 64

struct _private {
    
    int head, tail;
    int used, max;
    
    char **block;
};

/* Forward Declaration of static Functions */

static void set(struct _queue *this, char *str);

/* --- Extern Functions --- */

struct _queue* queue_ctor(void) {

    struct _queue *q;

    if((q = malloc(sizeof(struct _queue))) == NULL)
        return NULL;

    if((q->p = malloc(sizeof(struct _private))) == NULL) {

        free(q);
        return NULL;
    }

    q->p->block = malloc(INIT_SIZE * sizeof(char*));
    
    for(int i = 0; i < INIT_SIZE; i++)
        q->p->block[i] = NULL;
    
    q->p->head = q->p->tail = 0;
    
    q->p->used = 0;
    q->p->max = INIT_SIZE;
    
    return q;
}

void queue_dtor(struct _queue *this) {

    for(int i = 0; i < this->p->max; i++) {
     
        if(this->p->block[i] != NULL)
            free(this->p->block[i]);
    }

    free(this->p->block);
    free(this->p);
    free(this);
}

void queue_put(struct _queue *this, const int n, ...) {

    va_list var;
    va_start(var, n);

    if(this->p->used + n >= this->p->max)
        queue_resize(this, (this->p->max * 2) + n);
    
    for(int i = 0; i < n; i++) {

        char *str = va_arg(var, char*);
        set(this, str);
        
        this->p->tail = ((this->p->tail + 1) % this->p->max);
    }
    
    this->p->used += n;
    va_end(var);
}

char* queue_get(const struct _queue *this) {

    if(this->p->used == 0)
        return NULL;

    char *str = this->p->block[this->p->head];
    const int len = strlen(str);
    
    char *buffer = malloc((len + 1) * sizeof(char));
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    
    this->p->head = ((this->p->head + 1) % this->p->max);
    this->p->used -= 1;
    
    return buffer;
}

char* queue_str(const struct _queue *this) {
    
    const int max = this->p->max;
    const int head = this->p->head;
    
    const int len = queue_size(this);
    
    char *buffer = malloc((len + 1) * sizeof(char));
    buffer[0] = '\0';

    for(int i = 0; i < this->p->used; i++)
        strcat(buffer, this->p->block[(head + i) % max]);

    buffer[len] = '\0';
    return buffer;
}

void queue_flush(const struct _queue *this) {
    
    const int max = this->p->max;
    const int head = this->p->head;
    
    for(int i = 0; i < this->p->used; i++) {
        
        const int index = ((head + i) % max);
    
        if(this->p->block[index] == NULL)
            continue;
        
        free(this->p->block[index]);
        this->p->block[index] = NULL;
    }
    
    this->p->used = 0;
    this->p->head = this->p->tail = 0;
}

int queue_size(const struct _queue *this) {

    int size = 0;
    
    const int max = this->p->max;
    const int head = this->p->head;
    
    for(int i = 0; i < this->p->used; i++) {
     
        char *str = this->p->block[(head + i) % max];
        size += strlen(str);
    }
    
    return size;
}

void queue_resize(struct _queue *this, const int n) {
    
    if(n <= this->p->max)
        return;
    
    this->p->block = realloc(this->p->block, n * sizeof(char*));
    
    for(int i = this->p->max; i < n; i++)
        this->p->block[i] = NULL;
    
    this->p->max = n;
}

/* --- Static Functions --- */

static void set(struct _queue *this, char *str) {
    
    const int x = this->p->tail;
    
    if(this->p->block[x] != NULL)
        free(this->p->block[x]);
    
    const int len = strlen(str);
    
    this->p->block[x] = malloc((len + 1) * sizeof(char));
    strncpy(this->p->block[x], str, len);
    this->p->block[x][len] = '\0';
}


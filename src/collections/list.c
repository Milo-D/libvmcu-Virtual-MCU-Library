/* Generic List Implementation */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "collections/list.h"

struct _node {

    void *value;
    size_t size;

    struct _node *next;
};

struct _private {

    struct _node *tail;
    struct _node *head;

    void (*ffp)(void *fp);
    void* (*cfp)(void *cp);
};

/* Forward Declaration of Private Functions */

static struct _node* node_ctor(void *data, const size_t size, void* (*cfp)(void *fp));
static void node_dtor(struct _node *node,  void (*ffp)(void *fp));
static struct _node* ls_iterate(struct _list *this, const int pos);

/* Public Functions of Generic List */

extern struct _list* ls_ctor(const void *ffp, const void *cfp) {

    struct _list *ls;

    if((ls = malloc(sizeof(struct _list))) == NULL)
        return NULL;

    if((ls->p = malloc(sizeof(struct _private))) == NULL) {

        free(ls);
        return NULL;
    }

    const int init_data = 1;
    const size_t init_size = sizeof(int);

    ls->p->tail = node_ctor((void*) &init_data, init_size, NULL);
    ls->p->head = ls->p->tail;
    
    ls->p->ffp = ffp;
    ls->p->cfp = cfp;
    ls->size = 0;

    return ls;
}

extern struct _list* ls_cpy(struct _list *src) {

    struct _list *dest;
    
    if((dest = ls_ctor(src->p->ffp, src->p->cfp)) == NULL)
        return NULL;
    
    for(int i = 0; i < src->size; i++) {
     
        struct _node *it = ls_iterate(src, i);
        ls_push(dest, it->value, it->size);
    }

    return dest;
}

extern void ls_dtor(struct _list *this) {

    struct _node *it = this->p->tail->next;
    
    for(int i = 0; i < this->size; i++) {
        
        struct _node *tmp_next = it->next;
        
        node_dtor(it, this->p->ffp);
        it = tmp_next;
    }
    
    node_dtor(this->p->tail, NULL);

    free(this->p);
    free(this);
}

extern int ls_push(struct _list *this, void *data, const size_t size) {

    if((this->p->head->next = node_ctor(data, size, this->p->cfp)) == NULL)
        return -1;

    this->p->head = this->p->head->next;
    this->size += 1;

    return 0;
}

extern void* ls_pop(struct _list *this) {

    struct _node *last;
    
    if(this->size < 1)
        return NULL;
    
    if((last = ls_iterate(this, this->size - 2)) == NULL)
        return NULL;
    
    void *value;
    const size_t bytes = this->p->head->size;
    
    if(this->p->cfp != NULL) {

        value = (void*) (*this->p->cfp)(this->p->head->value);

    } else {

        value = malloc(bytes);
        memcpy(value, this->p->head->value, bytes);
    }
    
    node_dtor(this->p->head, this->p->ffp);
    this->p->head = last;
    this->size -= 1;
    
    return value;
}

extern int ls_add(struct _list *this, void *data, const size_t size, const int pos) {   // fix this
    
    struct _node *it;
    
    if((it = ls_iterate(this, pos)) == NULL)
        return -1;
    
    if(this->p->ffp != NULL)
        (*this->p->ffp)(it->value);
    else
        free(it->value);
    
    it->value = malloc(size);
    it->size = size;
    
    if(this->p->cfp != NULL)
        it->value = (void*) (*this->p->cfp)(data);
    else
        memcpy(it->value, data, size);

    return 0;
}

extern int ls_del(struct _list *this, const int pos) {      // this functions needs a fix

    struct _node *prev, *it;
    
    if(pos == 0)
        prev = this->p->tail;
    else if((prev = ls_iterate(this, pos - 1)) == NULL)
        return -1;
    
    if((it = ls_iterate(this, pos)) == NULL)
        return -1;
    
    prev->next = it->next;
    node_dtor(it, this->p->ffp);
    this->size -= 1;
    
    return 0;
}

extern int ls_insert(struct _list *this, void *data, const size_t size, const int pos) {
    
    struct _node *prev, *it;
    
    if(pos == 0 && this->size > 0) {
        
        struct _node *node = node_ctor(data, size, this->p->cfp);
        struct _node *tmp_next = this->p->tail->next;
        
        this->p->tail->next = node;
        node->next = tmp_next;
        
        this->size += 1;
        return 0;
    }
    
    if((prev = ls_iterate(this, pos - 1)) == NULL)
        return -1;
    
    if((it = ls_iterate(this, pos)) == NULL)
        return -1;
    
    struct _node *node = node_ctor(data, size, this->p->cfp);
    
    prev->next = node;
    node->next = it;
    
    this->size += 1;
    return 0;
}

extern void* ls_at(struct _list *this, const int pos) {

    struct _node *it;
    
    if((it = ls_iterate(this, pos)) == NULL)
        return NULL;
    
    return it->value;
}

/* Private Functions of Node */

static struct _node* node_ctor(void *data, const size_t size, void* (*cfp)(void *cp)) {

    struct _node *node;

    if((node = malloc(sizeof(struct _node))) == NULL)
        return NULL;
    
    if(cfp != NULL) {

        node->value = (void*) (*cfp)(data);

    } else {

        node->value = malloc(size);
        memcpy(node->value, data, size);
    }
    
    node->size = size;
    node->next = NULL;

    return node;
}

static void node_dtor(struct _node *node, void (*ffp)(void *fp)) {

    if(ffp != NULL)
        (*ffp)(node->value);
    else
        free(node->value);

    free(node);
}

static struct _node* ls_iterate(struct _list *this, const int pos) {
 
    struct _node *it = this->p->tail->next;
    
    for(int i = 0; i < this->size; i++) {
        
        if(it == NULL)
            return NULL;
        
        if(i == pos)
            return it;
        
        it = it->next;
    }
    
    return NULL;
}










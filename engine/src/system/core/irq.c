/* IRQ Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "system/core/irq.h"

/* Forward Declaration of static Functions */

static struct _request* new_request(const uint16_t isr);

/* --- Extern --- */

struct _irq* irq_ctor(void) {

    struct _irq *irq;

    if((irq = malloc(sizeof(struct _irq))) == NULL)
        return NULL;

    irq->head = NULL;
    irq->size = 0;

    return irq;
}

void irq_dtor(struct _irq *this) {

    struct _request *it = this->head;
    struct _request *next = it;

    while(it != NULL && this->size != 0) {

        next = it->next;
        free(it);

        it = next;
    }

    free(this);
}

int irq_enable(struct _irq *this, const uint16_t isr_vect) {

    if(irq_contains(this, isr_vect) == 0)
        return -1;

    if(this->head == NULL) {

        this->head = new_request(isr_vect);
        this->size += 1;
        
        return 0;
    }

    struct _request *it = this->head;
    struct _request *req = new_request(isr_vect);
    
    if(this->head->isr > isr_vect) {
    
        req->next = this->head;
        this->head = req;
        
        this->size += 1;
        return 0;
    }
    
    while(it->next != NULL && it->next->isr < isr_vect)
        it = it->next;

    req->next = it->next;
    it->next = req;

    this->size += 1;
    return 0;
}

int irq_disable(struct _irq *this, const uint16_t isr_vect) {

    if(irq_contains(this, isr_vect) < 0)
        return -1;

    struct _request *it = this->head;
    struct _request *it_prev = it;

    while(it != NULL) {
        
        if(it->isr == isr_vect) {
            
            it_prev->next = it->next;
            
            if(it == this->head)
                this->head = it->next;
                
            free(it);
            this->size -= 1;
            
            return 0;
        }            
        
        it_prev = it;             
        it = it->next;        
    }
    
    return -1;
}

int irq_pop(struct _irq *this, uint16_t *buffer) {

    if(this->size == 0)
        return -1;

    *buffer = this->head->isr;

    struct _request *temp = this->head;
    this->head = this->head->next;
    
    free(temp);
    this->size -= 1;
    
    return 0;
}

int irq_contains(const struct _irq *this, const uint16_t isr_vect) {
    
    if(this->size < 0)
        return -1;

    struct _request *it = this->head;
        
    while(it != NULL && isr_vect != it->isr)
        it = it->next;

    return (it == NULL) ? -1 : 0; 
}

void irq_reboot(struct _irq *this) {

    struct _request *it = this->head;
    struct _request *next = it;

    while(it != NULL) {

        next = it->next;
        free(it);

        it = next;
    }

    this->head = NULL;
    this->size = 0;
}

/* --- Static --- */

static struct _request* new_request(const uint16_t isr) {

    struct _request *request = malloc(sizeof(struct _request));

    request->isr = isr;
    request->next = NULL;

    return request;
}

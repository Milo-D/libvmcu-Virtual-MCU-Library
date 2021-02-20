/* IRQ Implementation */

// C Headers
#include <stdlib.h>

// Project Headers (engine)
#include "engine/include/system/core/irq.h"

/* Forward Declaration of static Functions */

static vmcu_request_t* new_request(const uint16_t isr);

/* --- Extern --- */

vmcu_irq_t* vmcu_irq_ctor(void) {

    vmcu_irq_t *irq;

    if((irq = malloc(sizeof(vmcu_irq_t))) == NULL)
        return NULL;

    irq->head = NULL;
    irq->size = 0;

    return irq;
}

void vmcu_irq_dtor(vmcu_irq_t *this) {

    vmcu_request_t *it = this->head;
    vmcu_request_t *next = it;

    while(it != NULL && this->size != 0) {

        next = it->next;
        free(it);

        it = next;
    }

    free(this);
}

int vmcu_irq_enable(vmcu_irq_t *this, const uint16_t isr_vect) {

    if(vmcu_irq_contains(this, isr_vect) == 0)
        return -1;

    if(this->head == NULL) {

        this->head = new_request(isr_vect);
        this->size += 1;
        
        return 0;
    }

    vmcu_request_t *it = this->head;
    vmcu_request_t *req = new_request(isr_vect);
    
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

int vmcu_irq_disable(vmcu_irq_t *this, const uint16_t isr_vect) {

    if(vmcu_irq_contains(this, isr_vect) < 0)
        return -1;

    vmcu_request_t *it = this->head;
    vmcu_request_t *it_prev = it;

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

int vmcu_irq_pop(vmcu_irq_t *this, uint16_t *buffer) {

    if(this->size == 0)
        return -1;

    *buffer = this->head->isr;

    vmcu_request_t *temp = this->head;
    this->head = this->head->next;
    
    free(temp);
    this->size -= 1;
    
    return 0;
}

int vmcu_irq_contains(const vmcu_irq_t *this, const uint16_t isr_vect) {
    
    if(this->size < 0)
        return -1;

    vmcu_request_t *it = this->head;
        
    while(it != NULL && isr_vect != it->isr)
        it = it->next;

    return (it == NULL) ? -1 : 0;
}

void vmcu_irq_reboot(vmcu_irq_t *this) {

    vmcu_request_t *it = this->head;
    vmcu_request_t *next = it;

    while(it != NULL) {

        next = it->next;
        free(it);

        it = next;
    }

    this->head = NULL;
    this->size = 0;
}

/* --- Static --- */

static vmcu_request_t* new_request(const uint16_t isr) {

    vmcu_request_t *request = malloc(sizeof(vmcu_request_t));

    request->isr = isr;
    request->next = NULL;

    return request;
}

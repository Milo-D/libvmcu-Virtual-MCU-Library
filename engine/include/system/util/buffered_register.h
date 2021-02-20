/* Double Buffered Register Header */

#ifndef VMCU_BUFFERED_REGISTER_H
#define VMCU_BUFFERED_REGISTER_H

// C Headers
#include <inttypes.h>

/*
 * This structure implements a double-buffered register,
 * commonly used in Timers.
 *
 * If a register is double-buffered, following steps
 * are mandatory:
 *
 * 1.) Declare an instance of buffered_register_t
 *
 * ex.      vmcu_buffered_register_t buffered_ocr;
 *
 * 2.) Initialize vmcu_buffered_register_t with an address
 * to the actual register, a comparison register and
 * its update condition.
 *
 * ex.      vmcu_buffered_register_t buffered_ocr;
 *
 *          buffered_ocr.reg = &ocr0a;
 *          buffered_ocr.cmp_with = &tcnt0;
 *          buffered_ocr.condition = TOP;
 *
 * 3.) Any further write access to the actual register
 * should use the vmcu_buffered_register_write() function
 * instead. This functions writes the value to register's buffer.
 *
 * ex.      void timer8_write_ocr(timer8_t *this, const int8_t value) {
 *
 *              if(NON_PWM_MODE) {
 *
 *                  this->ocr = value;
 *                  return;
 *              }
 *
 *              vmcu_buffered_register_write(&buffered_ocr, value);
 *          }
 *
 * 4.) The vmcu_buffered_register_update() function writes its
 * buffer to the actual register if a certain condition
 * (vmcu_buffered_register.condition) is met.
 *
 * ex.      void timer8_tick_fast_pwm(timer8_t *this, irq_t *irq) {
 *
 *              [...]
 *              buffered_register_update(&buffered_ocr);
 *              [...]
 *          }
 *
 * */

enum { VMCU_BUFFERED_REGISTER_WRITTEN, VMCU_BUFFERED_REGISTER_NOT_WRITTEN };

typedef struct vmcu_buffered_register {

    int8_t *reg;            /* Actual Register */
    int8_t buffer;          /* Double Buffer of actual Register */

    int8_t *cmp_with;       /* Comparison Register */
    int8_t condition;       /* Update Condition */

} vmcu_buffered_register_t;

extern void vmcu_buffered_register_write(vmcu_buffered_register_t *this, const int8_t value);
extern int vmcu_buffered_register_update(vmcu_buffered_register_t *this);

#endif
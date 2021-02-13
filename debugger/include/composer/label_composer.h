/* Disassembly Composer (Labels) Header */

#ifndef LABEL_COMPOSER_H
#define LABEL_COMPOSER_H

typedef struct _report report_t;
typedef struct _composed_disassembly composed_disassembly_t;

extern int compose_labels(const report_t *report, composed_disassembly_t *cdis);

#endif
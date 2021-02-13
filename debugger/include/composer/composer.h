/* Disassembly Composer Header */

#ifndef COMPOSER_H
#define COMPOSER_H

/*
 * This component tries to arrange raw
 * data from the analyzer report in order
 * to compose a nice and readable disassembly.
 * */

typedef struct _report report_t;
typedef struct _composed_disassembly composed_disassembly_t;

extern composed_disassembly_t* compose_disassembly(const report_t *report);

#endif
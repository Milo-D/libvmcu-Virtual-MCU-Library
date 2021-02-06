/* JSon Writer Header */

#ifndef JSONWRITER_H
#define JSONWRITER_H

typedef struct _queue queue_t;
typedef struct _system system_t;

extern int make_json(system_t *sys, queue_t *buffer);

#endif
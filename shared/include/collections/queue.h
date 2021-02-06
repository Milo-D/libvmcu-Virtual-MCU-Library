/* Char Queue Header */

#ifndef QUEUE_H
#define QUEUE_H

struct _private;

typedef struct _queue {

    struct _private *p;

} queue_t;

extern struct _queue* queue_ctor(void);
extern void queue_dtor(struct _queue *this);

extern void queue_put(struct _queue *this, const int n, ...);
extern char* queue_get(const struct _queue *this);
extern char* queue_str(const struct _queue *this);
extern void queue_flush(const struct _queue *this);

extern void queue_resize(struct _queue *this, const int n);
extern int queue_size(const struct _queue *this);

#endif
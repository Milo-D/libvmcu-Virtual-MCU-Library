/* String-UnsignedInteger Map Header */

#ifndef MAP_H
#define MAP_H

struct _private;

typedef struct _map {

    int size;
    int capacity;

    struct _private *p;

} map_t;

extern struct _map* map_ctor(const int n);
extern void map_dtor(struct _map *this);

extern int map_put(struct _map *this, const char *key, const int value);
extern int map_get(const struct _map *this, const char *key);

#endif

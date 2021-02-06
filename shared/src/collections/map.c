/* String-UnsignedInteger Map Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "collections/map.h"

typedef struct _pair {

    char *key;
    int value;

} pair_t;

struct _private {

    pair_t **grid;
};

/* Forward Declaration of private Functions */

static int hash(const char *seq, const int mod);
static int rotate(struct _map *this, const int pos);

/* --- Public --- */

struct _map* map_ctor(const int n) {

    struct _map *map;

    if((map = malloc(sizeof(struct _map))) == NULL)
        return NULL;

    if((map->p = malloc(sizeof(struct _private))) == NULL) {

        free(map);
        return NULL;
    }

    map->p->grid = malloc(n * sizeof(pair_t));

    for(int i = 0; i < n; i++)
        map->p->grid[i] = NULL;

    map->size = 0;
    map->capacity = n;

    return map;
}

void map_dtor(struct _map *this) {

    for(int i = 0; i < this->capacity; i++) {

        if(this->p->grid[i] != NULL) {
            
            free(this->p->grid[i]->key);
            free(this->p->grid[i]);
        }
    }

    free(this->p->grid);
    free(this->p);
    free(this);
}

int map_put(struct _map *this, const char *key, const int value) {

    int pos = hash(key, this->capacity);

    if(this->size == this->capacity)
        return -1;

    if(this->p->grid[pos] != NULL)
        pos = rotate(this, pos);

    if(pos < 0)
        return -1;

    this->p->grid[pos] = malloc(sizeof(pair_t));

    const size_t bytes = strlen(key);
    this->p->grid[pos]->key = malloc((bytes + 1) * sizeof(char));
    strncpy(this->p->grid[pos]->key, key, bytes);

    this->p->grid[pos]->key[bytes] = '\0';
    this->p->grid[pos]->value = value;

    return 0;
}

int map_get(const struct _map *this, const char *key) {

    int pos = hash(key, this->capacity);

    if(pos < 0)
        return -1;

    for(int i = 0; i < this->capacity; i++) {

        const int at = ((pos + i) % this->capacity);

        char *item = this->p->grid[at]->key;
        int value = this->p->grid[at]->value;

        if(item == NULL)
            continue;

        if(strcmp(item, key) == 0)
            return value;
    }

    return -1;
}

/* --- Private --- */

static int hash(const char *seq, const int mod) {

    int acc = 0;

    for(int i = 0; i < strlen(seq); i++)
        acc += seq[i];

    return (acc % mod);
}

static int rotate(struct _map *this, const int pos) {

    int i = 1;

    while(this->p->grid[(pos + i) % this->capacity] != NULL) {

        if(i == this->capacity)
            return -1;

        i += 1;
    }

    return ((pos + i) % this->capacity);
}

/* String Map Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (shared)
#include "shared/include/collections/strmap.h"

struct _private {

    char **grid;
};

/* Forward Declaration of private Functions */

static int hash(const char *seq, const int mod);
static int rotate(struct _strmap *this, const int pos);

/* --- Public --- */

struct _strmap* strmap_ctor(const int n) {

    struct _strmap *map;

    if((map = malloc(sizeof(struct _strmap))) == NULL)
        return NULL;

    if((map->p = malloc(sizeof(struct _private))) == NULL) {

        free(map);
        return NULL;
    }    

    map->p->grid = malloc(n * sizeof(char*));
    
    for(int i = 0; i < n; i++)
        map->p->grid[i] = NULL;

    map->size = 0;
    map->capacity = n;

    return map;
}

void strmap_dtor(struct _strmap *this) {

    for(int i = 0; i < this->capacity; i++) {

        char *e = this->p->grid[i];

        if(e != NULL)
            free(e);
    }

    free(this->p->grid);
    free(this->p);
    free(this);
}

int strmap_put(struct _strmap *this, const char *str) {

    int pos = hash(str, this->capacity);

    if(this->size == this->capacity)
        return -1;

    if(this->p->grid[pos] != NULL)
        pos = rotate(this, pos);

    if(pos < 0)
        return -1;

    const size_t bytes = strlen(str);
    this->p->grid[pos] = malloc((bytes + 1) * sizeof(char));

    strncpy(this->p->grid[pos], str, bytes);
    this->p->grid[pos][bytes] = '\0';
    this->size += 1;

    return 0;
}

int strmap_get(const struct _strmap *this, const char *str) {

    int pos = hash(str, this->capacity);

    if(pos < 0)
        return -1;

    for(int i = 0; i < this->capacity; i++) {

        char *item = this->p->grid[(pos + i) % this->capacity];

        if(item == NULL)
            continue;

        if(strcmp(item, str) == 0)
            return ((pos + i) % this->capacity);
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

static int rotate(struct _strmap *this, const int pos) {

    int i = 1;

    while(this->p->grid[(pos + i) % this->capacity] != NULL) {

        if(i == this->capacity)
            return -1;

        i += 1;
    }

    return ((pos + i) % this->capacity);
}
/* Plain (Data-Structure) Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers
#include "analyzer/report/plain.h"

void plain_dtor(struct _plain *this) {

    if(this->mnem != NULL)
        free(this->mnem);

    free(this);
}

struct _plain* plain_cpy(const struct _plain *src) {

    struct _plain *dest = malloc(sizeof(struct _plain));

    dest->opcode     = src->opcode;
    dest->addr       = src->addr;
    dest->key        = src->key;

    dest->exec       = src->exec;
    dest->dword      = src->dword;

    dest->src.type   = src->src.type;
    dest->src.value  = src->src.value;

    dest->dest.type  = src->dest.type;
    dest->dest.value = src->dest.value;

    if((dest->mnem = src->mnem) == NULL)
        return dest;

    const int len = strlen(src->mnem);

    dest->mnem = malloc((len + 1) * sizeof(char));
    strncpy(dest->mnem, src->mnem, len);
    dest->mnem[len] = '\0';

    return dest;
}

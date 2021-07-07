/* CFG (Data-Structure) Implementation */

// C Headers
#include <stdlib.h>

// Project Headers
#include "engine/include/analyzer/report/cfg.h"

vmcu_cfg_t* vmcu_cfg_ctor(const uint32_t n) {

    vmcu_cfg_t *cfg;

    if((cfg = malloc(sizeof(vmcu_cfg_t))) == NULL)
        return NULL;

    if((cfg->node = malloc(n * sizeof(vmcu_cfg_node_t))) == NULL) {

        free(cfg);
        return NULL;
    }

    cfg->used = 0;
    return cfg;
}

void vmcu_cfg_dtor(vmcu_cfg_t *this) {

    free(this->node);
    free(this);
}
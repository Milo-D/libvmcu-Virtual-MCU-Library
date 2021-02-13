/* Disassembly Composer (Labels) Implementation */

// C Headers
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/report/report.h"

// Project Headers (debugger)
#include "debugger/include/composer/cdis.h"

// Project Headers (shared)
#include "shared/include/misc/memmanip.h"
#include "shared/include/misc/stringmanip.h"
#include "shared/include/collections/sstream.h"

/*
 * Note: When calling cdis_assign_line I am
 * actually using the raw values of the enums
 * CDIS_NO_ADDR (-1) and CDIS_NO_OPC (0)
 * in order to keep the line length short.
 * */

/* Forward Declaration of static Functions */

static int insert_labels(const report_t *report, composed_disassembly_t *cdis);

static label_t* find_label(const report_t *report, const uint16_t addr);
static label_t* is_caller_of(const report_t *report, const uint16_t addr);

static char* generate_nl(void);
static char* generate_mnemonic(const char *mnem);
static char* generate_label(const label_t *label);
static char* generate_label_call(const char *mnem, const label_t *label);

/* --- Extern --- */

int compose_labels(const report_t *report, composed_disassembly_t *cdis) {

    /* upper bound of size */
    cdis->size += (2 * cdis->size) + 1;

    size_t bytes = cdis->size * sizeof(composed_line_t);
    cdis->line = realloc(cdis->line, bytes);

    insert_labels(report, cdis);

    /* adjusting size */
    bytes = cdis->size * sizeof(composed_line_t);
    cdis->line = realloc(cdis->line, bytes);

    return 0;
}

/* --- Static --- */

static int insert_labels(const report_t *report, composed_disassembly_t *cdis) {

    label_t *lx; int k = 0;

    if((lx = find_label(report, 0x0000)) != NULL) {

        char *str = generate_label(lx);
        cdis_assign_line(&cdis->line[k++], -1, 0, str);
    }

    for(int i = 0; i < report->progsize; i++) {

        plain_t *p = &report->disassembly[i];

        if((lx = is_caller_of(report, p->addr)) != NULL) {

            char *str = generate_label_call(p->mnem, lx);
            cdis_assign_line(&cdis->line[k++], p->addr, p->opcode, str);

        } else {

            char *str = generate_mnemonic(p->mnem);
            cdis_assign_line(&cdis->line[k++], p->addr, p->opcode, str);
        }

        if(i >= report->progsize - 1)
            continue;

        plain_t *q = &report->disassembly[i + 1];

        if((lx = find_label(report, q->addr)) != NULL) {

            char *str = generate_nl();
            cdis_assign_line(&cdis->line[k++], -1, 0, str);

            char *str2 = generate_label(lx);
            cdis_assign_line(&cdis->line[k++], -1, 0, str2);
        }
    }

    cdis->size = k;
    return 0;
}

static label_t* find_label(const report_t *report, const uint16_t addr) {

    for(int i = 0; i < report->nlabels; i++) {

        label_t *label = &report->labels[i];

        if(label->addr == addr)
            return label;

        if(label->addr > addr)
            return NULL;
    }

    return NULL;
}

static label_t* is_caller_of(const report_t *report, const uint16_t addr) {

    for(int i = 0; i < report->nlabels; i++) {

        for(int j = 0; j < report->labels[i].ncallers; j++) {

            if(report->labels[i].caller[j].addr == addr)
                return &report->labels[i];
        }
    }

    return NULL;
}

static char* generate_nl(void) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "");
    return sstream_alloc(&ss);
}

static char* generate_mnemonic(const char *mnem) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "%s", mnem);
    return sstream_alloc(&ss);
}

static char* generate_label(const label_t *label) {

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "L%d:", label->id);
    return sstream_alloc(&ss);
}

static char* generate_label_call(const char *mnem, const label_t *label) {

    const size_t pos = strpos(mnem, " ");
    const size_t len = strlen(mnem);

    char *mnemonic = substr(mnem, 0, pos - 1);
    char *trash = substr(mnem, pos + 1, len);

    const int tpos = strpos(trash, " ");
    char *comment = substr(trash, tpos + 1, len);

    sstream_t lx_builder;
    sstream_ctor(&lx_builder);

    sstream_put(&lx_builder, " L%d", label->id);

    size_t total = (len - (strlen(mnemonic) + strlen(comment)));
    total -= (lx_builder.length - 1);

    sstream_t ss;
    sstream_ctor(&ss);

    sstream_put(&ss, "%s%s", mnemonic, lx_builder.str);
    sstream_pad(&ss, total - 1);

    sstream_put(&ss, comment);
    nfree(3, mnemonic, trash, comment);

    return sstream_alloc(&ss);
}









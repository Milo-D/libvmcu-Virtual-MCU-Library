/* Multibyte Comparison Pattern Driver */

// C Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// libvmcu Headers
#include "libvmcu_analyzer.h"

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define LIST_SIZE_DFL 8

#define mnemlen(mnem) strlen(mnem->base) \
                    + strlen(mnem->src)  \
                    + strlen(mnem->dest)

/*
 * Multibyte Comparison Pattern
 * -------------------------------
 * This driver is supposed to find multibyte comparisons
 * in AVR binaries.
 *
 * The AVR instruction set has no instructions for comparing
 * multiple bytes. Thus, the compiler has to provide such
 * an option.
 *
 * AVR-GCC does this by chaining a single "cpi" or "cp" to one "cpc"
 * instruction per additional byte we want to compare.
 *
 * Therefore, a consecutive n-byte comparison pattern is defined as
 * p(k = 1) = cpi v cp and p(k > 1, k <= n) = cpc
 *
 * Here are some examples for consecutive n-byte comparisons:
 *
 * [1-byte consecutive comparison] cpi r26, 0x1e
 *
 * ==> comparing r26 with 0x1e
 *
 * [2-byte consecutive comparison] cpi r26, 0x1e
 *                                 cpc r27, r1
 *
 * ==> comparing r27:r26 with r1:0x1e
 *
 * [4-byte consecutive comparison] cpi r26, 0x1e
 *                                 cpc r27, r1
 *                                 cpc r28, r1
 *                                 cpc r29, r1
 *
 * ==> comparing r29:r28:r27:r26 with r1:r1:r1:0x1e
 *
 * Furthermore, we can safely assume that, an instruction i with no
 * implicit/explicit access to the status register, will not affect the
 * outcome of a multibyte comparison. For example,
 *
 * cpi r26, 0x1e
 * nop
 * ldi r26, 0xff
 * nop
 * cpc r27, r1
 *
 * will still be a valid 2-byte comparison of r27:r26 and r1:0x1e
 * since "nop" and "ldi" have both no impact on the CPU's status flags.
 *
 * This pattern is called a non-consecutive n-byte comparison and is defined as
 * p(k = 1) = cpi v cp and p(k > 1, k <= m - 1) = cpc v i and p(k = m) = cpc,
 * with i being an instruction with no access to status flags and m <= n.
 *
 * This piece of code uses libvmcu to detect non-consecutive multibyte
 * comparisons.
 *
 * */

/* Helper Structures */

typedef struct instr_list {

    uint32_t size;
    uint32_t index;

    vmcu_instr_t **item;

} instr_list_t;

/* Forward Declaration of static Functions */

static bool affects_flags(const vmcu_instr_t *instr);

static uint32_t max_addr(const vmcu_report_t *report);
static uint32_t traverse_cfg(const vmcu_cfg_node_t *root, instr_list_t *list, bool *hitmap);
static void print_pattern_details(const instr_list_t *pattern, const uint32_t bit);

static instr_list_t* instr_list_ctor(const uint32_t initial_size);
static void instr_list_dtor(instr_list_t *this);
static void instr_list_push_item(instr_list_t *this, vmcu_instr_t *item);
static void instr_list_reset(instr_list_t *this);

static bool* hitmap_ctor(const uint32_t initial_size);
static void hitmap_dtor(bool *this);

static void print_instruction(vmcu_instr_t *instr);
static void print_instruction_details(vmcu_instr_t *instr);
static void print_colored_base(const char *basestr, VMCU_GROUP group);
static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype);
static void print_padding(const size_t length, const size_t max);

/* --- Extern --- */

int main(const int argc, const char **argv) {

    if(argc != 2) {

        printf("Usage: ./cfg <binary>\n");
        return EXIT_FAILURE;
    }

    vmcu_model_t *m328p = vmcu_model_ctor(VMCU_DEVICE_M328P);
    vmcu_report_t *report = vmcu_analyze_file(argv[1], m328p);

    if(report == NULL) {

        vmcu_model_dtor(m328p);
        return EXIT_FAILURE;
    }

    instr_list_t *list = instr_list_ctor(LIST_SIZE_DFL);
    bool *hitmap = hitmap_ctor( max_addr(report) + 1 );

    for(uint32_t i = 0; i < report->cfg->used; i++) {

        vmcu_cfg_node_t *node = &report->cfg->node[i];
        VMCU_IKEY instr_key = node->xto.i->key;

        if(instr_key == VMCU_IKEY_CP || instr_key == VMCU_IKEY_CPI) {

            const uint32_t bit = traverse_cfg(node, list, hitmap);

            print_pattern_details(list, bit + 1);
            instr_list_reset(list);
        }
    }

    instr_list_dtor(list);
    hitmap_dtor(hitmap);

    vmcu_report_dtor(report);
    vmcu_model_dtor(m328p);

    return EXIT_SUCCESS;
}

/* --- Static --- */

static bool affects_flags(const vmcu_instr_t *instr) {

    return instr->writes.c_flag || instr->writes.z_flag ||
           instr->writes.n_flag || instr->writes.v_flag ||
           instr->writes.s_flag || instr->writes.h_flag ||
           instr->writes.t_flag || instr->writes.i_flag;
}

static uint32_t max_addr(const vmcu_report_t *report) {

    uint32_t max = 0;

    for(uint32_t i = 0; i < report->progsize; i++) {

        if(max < report->disassembly[i].addr)
            max = report->disassembly[i].addr;
    }

    return max;
}

static uint32_t traverse_cfg(const vmcu_cfg_node_t *root, instr_list_t *list, bool *hitmap) {

    if(hitmap[ root->xto.i->addr ] == true)
        return 0;

    uint32_t t_sum = 0, f_sum = 0;
    hitmap[ root->xto.i->addr ] = true;

    if(affects_flags(root->xto.i) == true)
        instr_list_push_item(list, root->xto.i);

    if(root->t != NULL && root->t->xto.i->key == VMCU_IKEY_CPC)
        t_sum = 1 + traverse_cfg(root->t, list, hitmap);

    if(root->t != NULL && affects_flags(root->t->xto.i) == false)
        t_sum = 0 + traverse_cfg(root->t, list, hitmap);

    if(root->f != NULL && root->f->xto.i->key == VMCU_IKEY_CPC)
        f_sum = 1 + traverse_cfg(root->f, list, hitmap);

    if(root->f != NULL && affects_flags(root->f->xto.i) == false)
        f_sum = 0 + traverse_cfg(root->f, list, hitmap);

    return (t_sum + f_sum);
}

static void print_pattern_details(const instr_list_t *pattern, const uint32_t bit) {

    printf("<--- PATTERN START --->\n");
    printf("\t<--- [ %d-bit comparison pattern detected ] --->\n", (bit * 8));

    for(uint32_t i = 0; i < bit; i++) {

        printf("\t\t");
        print_instruction(pattern->item[i]);
    }

    printf("\t<--- Details --->\n\t\tComparing ");

    for(int64_t i = bit - 1; i != -1; i--) {

        vmcu_instr_t *instr = pattern->item[i];
        printf("r%d", instr->dest.r);

        if(instr->dest.r == VMCU_REGISTER_R1)
            printf("(0x00?)");

        if(i != 0)
            printf(":");
    }

    printf(" with ");

    for(int64_t i = bit - 1; i != -1; i--) {

        vmcu_instr_t *instr = pattern->item[i];

        if(instr->src.type == VMCU_OPTYPE_R) {

            printf("r%d", instr->src.r);

            if(instr->src.r == VMCU_REGISTER_R1)
                printf("(0x00?)");
        }

        if(instr->src.type == VMCU_OPTYPE_K8)
            printf("0x%02x", instr->src.k);

        if(i != 0)
            printf(":");
    }

    printf("\n<--- PATTERN END --->\n\n");
}

static instr_list_t* instr_list_ctor(const uint32_t initial_size) {

    instr_list_t *instr_list;

    if((instr_list = malloc(sizeof(instr_list_t))) == NULL)
        return NULL;

    if((instr_list->item = malloc(initial_size * sizeof(vmcu_instr_t*))) == NULL) {

        free(instr_list);
        return NULL;
    }

    instr_list->size  = initial_size;
    instr_list->index = 0;

    return instr_list;
}

static void instr_list_dtor(instr_list_t *this) {

    free(this->item);
    free(this);
}

static void instr_list_push_item(instr_list_t *this, vmcu_instr_t *item) {

    if(this->index >= this->size) {

        this->size = this->size * 2;
        this->item = realloc(this->item, this->size * sizeof(vmcu_instr_t*));
    }

    this->item[ this->index++ ] = item;
}

static void instr_list_reset(instr_list_t *this) {

    this->index = 0;
}

static bool* hitmap_ctor(const uint32_t initial_size) {

    bool *hitmap = malloc((initial_size + 1) * sizeof(bool));
    memset(hitmap, false, (initial_size + 1) * sizeof(bool));

    return hitmap;
}

static void hitmap_dtor(bool *this) {

    free(this);
}

static void print_instruction(vmcu_instr_t *instr) {

    vmcu_mnemonic_t *mnem = &instr->mnem;

    print_instruction_details(instr);
    print_colored_base(mnem->base, instr->group);

    size_t sz = 0;

    if(instr->dest.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->dest, instr->dest.type);
        sz += printf(", ");
    }

    if(instr->src.type != VMCU_OPTYPE_NONE) {

        print_colored_operands(mnem->src, instr->src.type);
        sz += printf(" ");
    }

    print_padding(mnemlen(mnem) + sz, 26);
    printf("%s%s%s\n", COLOR_RED, mnem->comment, COLOR_RESET);
}

static void print_instruction_details(vmcu_instr_t *instr) {

    printf("0x%04" PRIx32, instr->addr);

    const uint16_t opcl = (instr->opcode & 0x0000ffff);
    const uint16_t swpl = (opcl >> 8) | (opcl << 8);

    const uint16_t opch = ((instr->opcode & 0xffff0000) >> 16);
    const uint16_t swph = (opch >> 8) | (opch << 8);

    if(instr->dword == false)
        printf("      %s....%s ", COLOR_GREEN, COLOR_RESET);
    else
        printf("      %s%04" PRIx16 "%s ", COLOR_GREEN, swph, COLOR_RESET);

    printf("%s%04" PRIx16 "%s      ", COLOR_YELLOW, swpl, COLOR_RESET);
}

static void print_colored_base(const char *basestr, VMCU_GROUP group) {

    switch(group) {

        case VMCU_GROUP_NONE: printf("%s", COLOR_YELLOW);    break;
        case VMCU_GROUP_MATH_LOGIC: printf("%s", COLOR_RED); break;
        case VMCU_GROUP_SYS_CTRL: printf("%s", COLOR_CYAN);  break;
        case VMCU_GROUP_TRANSFER: printf("%s", COLOR_RESET); break;
        case VMCU_GROUP_FLOW: printf("%s", COLOR_GREEN);     break;
        case VMCU_GROUP_BIT: printf("%s", COLOR_MAGENTA);    break;

        default: break;
    }

    printf("%s%s ", basestr, COLOR_RESET);
}

static void print_colored_operands(const char *opstr, VMCU_OPTYPE optype) {

    switch(optype) {

        case VMCU_OPTYPE_B:
        case VMCU_OPTYPE_K4:
        case VMCU_OPTYPE_K6:
        case VMCU_OPTYPE_K8:
        case VMCU_OPTYPE_IO5:
        case VMCU_OPTYPE_IO6:
        case VMCU_OPTYPE_D7:
        case VMCU_OPTYPE_D16:
        case VMCU_OPTYPE_P22:
        case VMCU_OPTYPE_S7:
        case VMCU_OPTYPE_S12:

            printf("%s", COLOR_BLUE);

        break;

        case VMCU_OPTYPE_R:
        case VMCU_OPTYPE_RP:
        case VMCU_OPTYPE_X:
        case VMCU_OPTYPE_Y:
        case VMCU_OPTYPE_Z:

            printf("%s", COLOR_CYAN);

        break;

        default: break;
    }

    printf("%s%s", opstr, COLOR_RESET);
}

static void print_padding(const size_t length, const size_t max) {

    if(length >= max)
        return;

    char pad[max - length + 1];

    memset(pad, ' ', (max - length));
    pad[max - length] = '\0';

    printf("%s", pad);
}

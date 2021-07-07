/* Controlflow Analyzer Submodule Implementation */

// C Headers
#include <stdlib.h>
#include <inttypes.h>

// Project Headers (engine)
#include "engine/include/analyzer/modules/controlflow/controlflow_analyzer.h"
#include "engine/include/analyzer/report/report.h"

// Project Headers (engine utilities)
#include "engine/include/arch/model.h"
#include "engine/include/misc/nummanip.h"

/*
 * controlflow graph
 * ---------------------------------
 * A controlflow graph (referred to as cfg) holds a set of controlflow nodes (cfg node).
 * Currently the graph is modeled as a directed graph.
 *
 * controlflow node
 * ---------------------------------
 * Each cfg node has exactly two edges (possible execution paths) (true = t, false = f).
 * The default path of each node is the true (t) edge. If a specific execution path does
 * not exist, the corresponding node pointer will be set to NULL.
 *
 * Additionally, a cfg node is equipped with the vmcu_xref_t (xto, xref to) structure. This
 * xref (xto) points to the corresponding instruction in the disassembly.
 *
 * leaf instruction
 * ---------------------------------
 * A leaf instruction i is an instruction whose controlflow node has following properties
 *
 * [1] .xto->i = &i     (node refers to i)
 * [2] .t      = NULL   (true branch = NULL, no false branch)
 * [3] .f      = NULL   (false branch = NULL, no true branch)
 *
 * Since the true branch is the default branch, we can remove [3] statement from our definition.
 *
 * [1] .xto->i = &i     (node refers to i)
 * [2] .t      = NULL   (true branch = NULL, no false branch)
 *
 * handling indirect controlflow
 * ---------------------------------
 * indirect changes in controlflow (ijmp, etc.) are handled as leaf instructions due to their
 * dependence on runtime data.
 *
 * handling flow out of disassembly
 * ---------------------------------
 * if an instruction attempts to change the flow to an address out of disassembly, its branch
 * in the corresponding controlflow graph will be set to NULL.
 *
 * finding execution paths (branch)
 * ---------------------------------
 * rules to determine .t / .f branch in a cfg node
 *
 * disasm(a) := { NULL,               if a >= disasm.size
 *              { disassembly[a],     if a  < disasm.size
 *
 * [1] (i.group != FLOW) ==> .t = disasm(i.addr + 1), .f = NULL (non-flow, including illegal opcodes)
 * [2] (i.group = FLOW ^ i.src.type = i.dest.type = NONE) ==> .t = .f = NULL (flow, without operands)
 * [3] (i.src.type = S7) ==> .t = disasm(i.addr + i.src.s + 1), .f = disasm(i.addr + 1) (relative flow)
 * [4] (i.src.type = S12) ==> .t = disasm(i.addr + i.src.s + 1), .f = NULL (unconditional relative flow)
 * [5] (i.key = JMP v i.key = CALL) ==> .t = disasm(i.src.p), .f = NULL (32-bit absolute flow)
 * [6] (i.group = FLOW ^ i.src.type = B) ==> .t = disasm(i.addr + disasm(i.addr + 1).dword + 2)
 *                                           .f = disasm(i.addr + 1) (skip instructions)
 * [7] (i.key = CPSE) ==> [6]
 *
 * */

/* Internal Structures */

typedef struct branch_buffer {

    int32_t t;
    int32_t f;

} branch_buffer_t;

/* Forward Declaration of static Functions */

static void tf(vmcu_report_t *report, vmcu_instr_t *instr, vmcu_model_t *mcu, branch_buffer_t *b);

static vmcu_cfg_node_t* seek_node(const vmcu_cfg_t *cfg, const int32_t addr);
static vmcu_instr_t* peek(const vmcu_report_t *report, const int32_t addr);

/* --- Extern --- */

extern int vmcu_analyze_control_flow(vmcu_report_t *report, vmcu_model_t *mcu) {

    report->cfg = vmcu_cfg_ctor(report->progsize);

    for(int32_t i = 0; i < report->progsize; i++) {

        vmcu_instr_t *instr = &report->disassembly[i];
        report->cfg->node[ report->cfg->used++ ].xto.i = instr;
    }

    for(int32_t i = 0; i < report->cfg->used; i++) {

        vmcu_instr_t *instr = report->cfg->node[i].xto.i;

        branch_buffer_t b;
        tf(report, instr, mcu, &b);

        report->cfg->node[i].t = seek_node(report->cfg, b.t);
        report->cfg->node[i].f = seek_node(report->cfg, b.f);
    }

    return 0;
}

/* --- Static --- */

static void tf(vmcu_report_t *report, vmcu_instr_t *instr, vmcu_model_t *mcu, branch_buffer_t *b) {

    const VMCU_IKEY key    = instr->key;
    const VMCU_OPTYPE src  = instr->src.type;

    const int32_t a        = instr->addr;
    const int16_t s        = instr->src.s;
    const uint32_t f       = mcu->flash.size;

    if(instr->group != VMCU_GROUP_FLOW) {

        b->t = mod(a + 1, mcu->flash.size);
        b->f = -1;

        return;
    }

    if((key == VMCU_IKEY_JMP) || (key == VMCU_IKEY_CALL)) {

        b->t = instr->src.p;
        b->f = -1;

        return;
    }

    if((src == VMCU_OPTYPE_S7) || (src == VMCU_OPTYPE_S12)) {

        b->t = mod(a + s + 1, mcu->flash.size);
        b->f = (src == VMCU_OPTYPE_S7) ? mod(a + 1, mcu->flash.size) : -1;

        return;
    }

    if((src == VMCU_OPTYPE_B) || (key == VMCU_IKEY_CPSE)) {

        vmcu_instr_t *next = peek(report, mod(a + 1, f));

        b->t = (next != NULL) ? mod(a + next->dword + 2, f) : -1;
        b->f = mod(a + 1, f);

        return;
    }

    b->t = b->f = -1;
}

static vmcu_cfg_node_t* seek_node(const vmcu_cfg_t *cfg, const int32_t addr) {

    if(addr < 0)
        return NULL;

    for(int32_t i = 0; i < cfg->used; i++) {

        if(cfg->node[i].xto.i->addr == addr)
            return &cfg->node[i];
    }

    return NULL;
}

static vmcu_instr_t* peek(const vmcu_report_t *report, const int32_t addr) {

    for(int32_t i = 0; i < report->progsize; i++) {

        if(report->disassembly[i].addr == addr)
            return &report->disassembly[i];
    }

    return NULL;
}


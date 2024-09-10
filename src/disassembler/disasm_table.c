#include <stddef.h>

#include "disasm_table.h"
#include "handlers.h"

/*
 * A structure representing a single entry
 * within the following disassembly table
 * */
typedef struct disasm_table_entry {

    uint16_t         mask;
    uint16_t         static_opcode;
    disasm_handler_t handler;

} disasm_table_entry_t;

const disasm_table_entry_t disasm_table[VMCU_IID_ENUM_END] = {

    [VMCU_IID_NOP]     = { .mask = 0xffff, .static_opcode = 0x0000, .handler = disassemble_nop    },
    [VMCU_IID_MOVW]    = { .mask = 0xff00, .static_opcode = 0x0100, .handler = disassemble_movw   },
    [VMCU_IID_MUL]     = { .mask = 0xfc00, .static_opcode = 0x9c00, .handler = disassemble_mul    },
    [VMCU_IID_MULS]    = { .mask = 0xff00, .static_opcode = 0x0200, .handler = disassemble_muls   },
    [VMCU_IID_MULSU]   = { .mask = 0xff88, .static_opcode = 0x0300, .handler = disassemble_mulsu  },
    [VMCU_IID_FMUL]    = { .mask = 0xff88, .static_opcode = 0x0308, .handler = disassemble_fmul   },
    [VMCU_IID_FMULS]   = { .mask = 0xff88, .static_opcode = 0x0380, .handler = disassemble_fmuls  },
    [VMCU_IID_FMULSU]  = { .mask = 0xff88, .static_opcode = 0x0388, .handler = disassemble_fmulsu },
    [VMCU_IID_LDI]     = { .mask = 0xf000, .static_opcode = 0xe000, .handler = disassemble_ldi    },
    [VMCU_IID_RJMP]    = { .mask = 0xf000, .static_opcode = 0xc000, .handler = disassemble_rjmp   },
    [VMCU_IID_JMP]     = { .mask = 0xfe0e, .static_opcode = 0x940c, .handler = disassemble_jmp    },
    [VMCU_IID_IJMP]    = { .mask = 0xffff, .static_opcode = 0x9409, .handler = disassemble_ijmp   },
    [VMCU_IID_MOV]     = { .mask = 0xfc00, .static_opcode = 0x2c00, .handler = disassemble_mov    },
    [VMCU_IID_DEC]     = { .mask = 0xfe0f, .static_opcode = 0x940a, .handler = disassemble_dec    },
    [VMCU_IID_INC]     = { .mask = 0xfe0f, .static_opcode = 0x9403, .handler = disassemble_inc    },
    [VMCU_IID_ADD]     = { .mask = 0xfc00, .static_opcode = 0x0c00, .handler = disassemble_add    },
    [VMCU_IID_ADC]     = { .mask = 0xfc00, .static_opcode = 0x1c00, .handler = disassemble_adc    },
    [VMCU_IID_ADIW]    = { .mask = 0xff00, .static_opcode = 0x9600, .handler = disassemble_adiw   },
    [VMCU_IID_SUB]     = { .mask = 0xfc00, .static_opcode = 0x1800, .handler = disassemble_sub    },
    [VMCU_IID_SUBI]    = { .mask = 0xf000, .static_opcode = 0x5000, .handler = disassemble_subi   },
    [VMCU_IID_SBC]     = { .mask = 0xfc00, .static_opcode = 0x0800, .handler = disassemble_sbc    },
    [VMCU_IID_SBCI]    = { .mask = 0xf000, .static_opcode = 0x4000, .handler = disassemble_sbci   },
    [VMCU_IID_SBIW]    = { .mask = 0xff00, .static_opcode = 0x9700, .handler = disassemble_sbiw   },
    [VMCU_IID_PUSH]    = { .mask = 0xfe0f, .static_opcode = 0x920f, .handler = disassemble_push   },
    [VMCU_IID_POP]     = { .mask = 0xfe0f, .static_opcode = 0x900f, .handler = disassemble_pop    },
    [VMCU_IID_IN]      = { .mask = 0xf800, .static_opcode = 0xb000, .handler = disassemble_in     },
    [VMCU_IID_OUT]     = { .mask = 0xf800, .static_opcode = 0xb800, .handler = disassemble_out    },
    [VMCU_IID_SBIS]    = { .mask = 0xff00, .static_opcode = 0x9b00, .handler = disassemble_sbis   },
    [VMCU_IID_SBIC]    = { .mask = 0xff00, .static_opcode = 0x9900, .handler = disassemble_sbic   },
    [VMCU_IID_SBRC]    = { .mask = 0xfe08, .static_opcode = 0xfc00, .handler = disassemble_sbrc   },
    [VMCU_IID_SBRS]    = { .mask = 0xfe08, .static_opcode = 0xfe00, .handler = disassemble_sbrs   },
    [VMCU_IID_CPSE]    = { .mask = 0xfc00, .static_opcode = 0x1000, .handler = disassemble_cpse   },
    [VMCU_IID_EOR]     = { .mask = 0xfc00, .static_opcode = 0x2400, .handler = disassemble_eor    },
    [VMCU_IID_LDX]     = { .mask = 0xfe0f, .static_opcode = 0x900c, .handler = disassemble_ldx    },
    [VMCU_IID_LDXI]    = { .mask = 0xfe0f, .static_opcode = 0x900d, .handler = disassemble_ldxi   },
    [VMCU_IID_LDDX]    = { .mask = 0xfe0f, .static_opcode = 0x900e, .handler = disassemble_lddx   },
    [VMCU_IID_LDY]     = { .mask = 0xfe0f, .static_opcode = 0x8008, .handler = disassemble_ldy    },
    [VMCU_IID_LDYI]    = { .mask = 0xfe0f, .static_opcode = 0x9009, .handler = disassemble_ldyi   },
    [VMCU_IID_LDDY]    = { .mask = 0xfe0f, .static_opcode = 0x900a, .handler = disassemble_lddy   },
    [VMCU_IID_LDDYQ]   = { .mask = 0xd208, .static_opcode = 0x8008, .handler = disassemble_lddyq  },
    [VMCU_IID_LDDZQ]   = { .mask = 0xd208, .static_opcode = 0x8000, .handler = disassemble_lddzq  },
    [VMCU_IID_LDZ]     = { .mask = 0xfe0f, .static_opcode = 0x8000, .handler = disassemble_ldz    },
    [VMCU_IID_LDZI]    = { .mask = 0xfe0f, .static_opcode = 0x9001, .handler = disassemble_ldzi   },
    [VMCU_IID_LDDZ]    = { .mask = 0xfe0f, .static_opcode = 0x9002, .handler = disassemble_lddz   },
    [VMCU_IID_STX]     = { .mask = 0xfe0f, .static_opcode = 0x920c, .handler = disassemble_stx    },
    [VMCU_IID_STXI]    = { .mask = 0xfe0f, .static_opcode = 0x920d, .handler = disassemble_stxi   },
    [VMCU_IID_STDX]    = { .mask = 0xfe0f, .static_opcode = 0x920e, .handler = disassemble_stdx   },
    [VMCU_IID_STY]     = { .mask = 0xfe0f, .static_opcode = 0x8208, .handler = disassemble_sty    },
    [VMCU_IID_STYI]    = { .mask = 0xfe0f, .static_opcode = 0x9209, .handler = disassemble_styi   },
    [VMCU_IID_STDY]    = { .mask = 0xfe0f, .static_opcode = 0x920a, .handler = disassemble_stdy   },
    [VMCU_IID_STDYQ]   = { .mask = 0xd208, .static_opcode = 0x8208, .handler = disassemble_stdyq  },
    [VMCU_IID_STZ]     = { .mask = 0xfe0f, .static_opcode = 0x8200, .handler = disassemble_stz    },
    [VMCU_IID_STZI]    = { .mask = 0xfe0f, .static_opcode = 0x9201, .handler = disassemble_stzi   },
    [VMCU_IID_STDZ]    = { .mask = 0xfe0f, .static_opcode = 0x9202, .handler = disassemble_stdz   },
    [VMCU_IID_STDZQ]   = { .mask = 0xd208, .static_opcode = 0x8200, .handler = disassemble_stdzq  },
    [VMCU_IID_STS]     = { .mask = 0xf800, .static_opcode = 0xa800, .handler = disassemble_sts    },
    [VMCU_IID_STS32]   = { .mask = 0xfe0f, .static_opcode = 0x9200, .handler = disassemble_sts32  },
    [VMCU_IID_LDS]     = { .mask = 0xf800, .static_opcode = 0xa000, .handler = disassemble_lds    },
    [VMCU_IID_LDS32]   = { .mask = 0xfe0f, .static_opcode = 0x9000, .handler = disassemble_lds32  },
    [VMCU_IID_XCH]     = { .mask = 0xfe0f, .static_opcode = 0x9204, .handler = disassemble_xch    },
    [VMCU_IID_BRNE]    = { .mask = 0xfc07, .static_opcode = 0xf401, .handler = disassemble_brne   },
    [VMCU_IID_BREQ]    = { .mask = 0xfc07, .static_opcode = 0xf001, .handler = disassemble_breq   },
    [VMCU_IID_BRGE]    = { .mask = 0xfc07, .static_opcode = 0xf404, .handler = disassemble_brge   },
    [VMCU_IID_BRPL]    = { .mask = 0xfc07, .static_opcode = 0xf402, .handler = disassemble_brpl   },
    [VMCU_IID_BRLO]    = { .mask = 0xfc07, .static_opcode = 0xf000, .handler = disassemble_brlo   },
    [VMCU_IID_BRLT]    = { .mask = 0xfc07, .static_opcode = 0xf004, .handler = disassemble_brlt   },
    [VMCU_IID_BRCC]    = { .mask = 0xfc07, .static_opcode = 0xf400, .handler = disassemble_brcc   },
    [VMCU_IID_BRVS]    = { .mask = 0xfc07, .static_opcode = 0xf003, .handler = disassemble_brvs   },
    [VMCU_IID_BRTS]    = { .mask = 0xfc07, .static_opcode = 0xf006, .handler = disassemble_brts   },
    [VMCU_IID_BRTC]    = { .mask = 0xfc07, .static_opcode = 0xf406, .handler = disassemble_brtc   },
    [VMCU_IID_BRMI]    = { .mask = 0xfc07, .static_opcode = 0xf002, .handler = disassemble_brmi   },
    [VMCU_IID_BRHC]    = { .mask = 0xfc07, .static_opcode = 0xf405, .handler = disassemble_brhc   },
    [VMCU_IID_BRHS]    = { .mask = 0xfc07, .static_opcode = 0xf005, .handler = disassemble_brhs   },
    [VMCU_IID_BRID]    = { .mask = 0xfc07, .static_opcode = 0xf407, .handler = disassemble_brid   },
    [VMCU_IID_BRIE]    = { .mask = 0xfc07, .static_opcode = 0xf007, .handler = disassemble_brie   },
    [VMCU_IID_BRVC]    = { .mask = 0xfc07, .static_opcode = 0xf403, .handler = disassemble_brvc   },
    [VMCU_IID_RCALL]   = { .mask = 0xf000, .static_opcode = 0xd000, .handler = disassemble_rcall  },
    [VMCU_IID_RET]     = { .mask = 0xffff, .static_opcode = 0x9508, .handler = disassemble_ret    },
    [VMCU_IID_RETI]    = { .mask = 0xffff, .static_opcode = 0x9518, .handler = disassemble_reti   },
    [VMCU_IID_ICALL]   = { .mask = 0xffff, .static_opcode = 0x9509, .handler = disassemble_icall  },
    [VMCU_IID_CALL]    = { .mask = 0xfe0e, .static_opcode = 0x940e, .handler = disassemble_call   },
    [VMCU_IID_CP]      = { .mask = 0xfc00, .static_opcode = 0x1400, .handler = disassemble_cp     },
    [VMCU_IID_CPI]     = { .mask = 0xf000, .static_opcode = 0x3000, .handler = disassemble_cpi    },
    [VMCU_IID_CPC]     = { .mask = 0xfc00, .static_opcode = 0x0400, .handler = disassemble_cpc    },
    [VMCU_IID_LSR]     = { .mask = 0xfe0f, .static_opcode = 0x9406, .handler = disassemble_lsr    },
    [VMCU_IID_ASR]     = { .mask = 0xfe0f, .static_opcode = 0x9405, .handler = disassemble_asr    },
    [VMCU_IID_ROR]     = { .mask = 0xfe0f, .static_opcode = 0x9407, .handler = disassemble_ror    },
    [VMCU_IID_SWAP]    = { .mask = 0xfe0f, .static_opcode = 0x9402, .handler = disassemble_swap   },
    [VMCU_IID_ORI]     = { .mask = 0xf000, .static_opcode = 0x6000, .handler = disassemble_ori    },
    [VMCU_IID_OR]      = { .mask = 0xfc00, .static_opcode = 0x2800, .handler = disassemble_or     },
    [VMCU_IID_AND]     = { .mask = 0xfc00, .static_opcode = 0x2000, .handler = disassemble_and    },
    [VMCU_IID_ANDI]    = { .mask = 0xf000, .static_opcode = 0x7000, .handler = disassemble_andi   },
    [VMCU_IID_LAS]     = { .mask = 0xfe0f, .static_opcode = 0x9205, .handler = disassemble_las    },
    [VMCU_IID_LAC]     = { .mask = 0xfe0f, .static_opcode = 0x9206, .handler = disassemble_lac    },
    [VMCU_IID_LAT]     = { .mask = 0xfe0f, .static_opcode = 0x9207, .handler = disassemble_lat    },
    [VMCU_IID_COM]     = { .mask = 0xfe0f, .static_opcode = 0x9400, .handler = disassemble_com    },
    [VMCU_IID_NEG]     = { .mask = 0xfe0f, .static_opcode = 0x9401, .handler = disassemble_neg    },
    [VMCU_IID_BLD]     = { .mask = 0xfe08, .static_opcode = 0xf800, .handler = disassemble_bld    },
    [VMCU_IID_BST]     = { .mask = 0xfe08, .static_opcode = 0xfa00, .handler = disassemble_bst    },
    [VMCU_IID_SBI]     = { .mask = 0xff00, .static_opcode = 0x9a00, .handler = disassemble_sbi    },
    [VMCU_IID_CBI]     = { .mask = 0xff00, .static_opcode = 0x9800, .handler = disassemble_cbi    },
    [VMCU_IID_LPM]     = { .mask = 0xffff, .static_opcode = 0x95c8, .handler = disassemble_lpm    },
    [VMCU_IID_LPMZ]    = { .mask = 0xfe0f, .static_opcode = 0x9004, .handler = disassemble_lpmz   },
    [VMCU_IID_LPMZI]   = { .mask = 0xfe0f, .static_opcode = 0x9005, .handler = disassemble_lpmzi  },
    [VMCU_IID_EICALL]  = { .mask = 0xffff, .static_opcode = 0x9519, .handler = disassemble_eicall },
    [VMCU_IID_EIJMP]   = { .mask = 0xffff, .static_opcode = 0x9419, .handler = disassemble_eijmp  },
    [VMCU_IID_ELPM]    = { .mask = 0xffff, .static_opcode = 0x95d8, .handler = disassemble_elpm   },
    [VMCU_IID_ELPMZ]   = { .mask = 0xfe0f, .static_opcode = 0x9006, .handler = disassemble_elpmz  },
    [VMCU_IID_ELPMZI]  = { .mask = 0xfe0f, .static_opcode = 0x9007, .handler = disassemble_elpmzi },
    [VMCU_IID_DES]     = { .mask = 0xff0f, .static_opcode = 0x940b, .handler = disassemble_des    },
    [VMCU_IID_SLEEP]   = { .mask = 0xffff, .static_opcode = 0x9588, .handler = disassemble_sleep  },
    [VMCU_IID_WDR]     = { .mask = 0xffff, .static_opcode = 0x95a8, .handler = disassemble_wdr    },
    [VMCU_IID_BREAK]   = { .mask = 0xffff, .static_opcode = 0x9598, .handler = disassemble_break  },
    [VMCU_IID_SPM]     = { .mask = 0xffff, .static_opcode = 0x95e8, .handler = disassemble_spm    },
    [VMCU_IID_SPMZI]   = { .mask = 0xffff, .static_opcode = 0x95f8, .handler = disassemble_spmzi  },
    [VMCU_IID_SES]     = { .mask = 0xffff, .static_opcode = 0x9448, .handler = disassemble_ses    },
    [VMCU_IID_SET]     = { .mask = 0xffff, .static_opcode = 0x9468, .handler = disassemble_set    },
    [VMCU_IID_SEV]     = { .mask = 0xffff, .static_opcode = 0x9438, .handler = disassemble_sev    },
    [VMCU_IID_SEZ]     = { .mask = 0xffff, .static_opcode = 0x9418, .handler = disassemble_sez    },
    [VMCU_IID_SEH]     = { .mask = 0xffff, .static_opcode = 0x9458, .handler = disassemble_seh    },
    [VMCU_IID_SEC]     = { .mask = 0xffff, .static_opcode = 0x9408, .handler = disassemble_sec    },
    [VMCU_IID_SEI]     = { .mask = 0xffff, .static_opcode = 0x9478, .handler = disassemble_sei    },
    [VMCU_IID_SEN]     = { .mask = 0xffff, .static_opcode = 0x9428, .handler = disassemble_sen    },
    [VMCU_IID_CLS]     = { .mask = 0xffff, .static_opcode = 0x94c8, .handler = disassemble_cls    },
    [VMCU_IID_CLT]     = { .mask = 0xffff, .static_opcode = 0x94e8, .handler = disassemble_clt    },
    [VMCU_IID_CLV]     = { .mask = 0xffff, .static_opcode = 0x94b8, .handler = disassemble_clv    },
    [VMCU_IID_CLZ]     = { .mask = 0xffff, .static_opcode = 0x9498, .handler = disassemble_clz    },
    [VMCU_IID_CLH]     = { .mask = 0xffff, .static_opcode = 0x94d8, .handler = disassemble_clh    },
    [VMCU_IID_CLC]     = { .mask = 0xffff, .static_opcode = 0x9488, .handler = disassemble_clc    },
    [VMCU_IID_CLI]     = { .mask = 0xffff, .static_opcode = 0x94f8, .handler = disassemble_cli    },
    [VMCU_IID_CLN]     = { .mask = 0xffff, .static_opcode = 0x94a8, .handler = disassemble_cln    }
};

/* --- Exposed --- */

disasm_handler_t get_disasm_handler(const uint16_t bytes) {

    for (vmcu_iid_t i = VMCU_IID_NOP; i < VMCU_IID_ENUM_END; i++) {

        const disasm_table_entry_t* it = &disasm_table[i];

        if ((bytes & it->mask) == it->static_opcode)
            return it->handler;
    }

    return disassemble_invalid_opcode;
}

bool is_32bit_instruction(const uint16_t bytes) {

    const vmcu_iid_t all_32bit_instructions[] = {

        VMCU_IID_JMP,
        VMCU_IID_CALL,
        VMCU_IID_STS32,
        VMCU_IID_LDS32 
    };

    for (size_t i = 0; i < sizeof(all_32bit_instructions) / sizeof(vmcu_iid_t); i++) {

        const disasm_table_entry_t* entry = &disasm_table[ all_32bit_instructions[i] ];

        if ((bytes & entry->mask) == entry->static_opcode)
            return true;
    }

    return false;
}

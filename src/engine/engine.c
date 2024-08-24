#include <lib/engine.h>

void vmcu_engine_init(vmcu_engine_t* e) {

    *e = (vmcu_engine_t) {

        .instruction_set = VMCU_CORE_AVRep,

        .reader = {

            .file_format = VMCU_FMT_IHEX,
            .endianness  = VMCU_ENDIAN_LITTLE,
            .sort        = false
        },

        .disassembler = {

            .words_mutable = true
        },

        .assembler = {

            .expect_operand_structs = true
        }
    };
}


#include <string.h>

#include <lib/engine.h>
#include <lib/rc.h>

static const vmcu_engine_t default_engine = {

    .rc              = VMCU_RC_OK,
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

__thread vmcu_engine_t vmcu = {

    .rc              = default_engine.rc,
    .instruction_set = default_engine.instruction_set,

    .reader = {

        .file_format = default_engine.reader.file_format,
        .endianness  = default_engine.reader.endianness,
        .sort        = default_engine.reader.sort
    },

    .disassembler = {

        .words_mutable = default_engine.disassembler.words_mutable
    },

    .assembler = {

        .expect_operand_structs = default_engine.assembler.expect_operand_structs
    }
};

void vmcu_engine_reset(void) {

    memcpy(&vmcu, &default_engine, sizeof(vmcu_engine_t));
}

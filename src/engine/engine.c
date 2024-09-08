#include <stdlib.h>
#include <string.h>

#include <lib/engine.h>
#include <lib/rc.h>

static const vmcu_engine_t default_engine = {

    .rc              = VMCU_RC_OK,
    .instruction_set = VMCU_CORE_AVRep,

    .cb = {

        .alloc = malloc,
        .free  = free
    },

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
    },

    .writer = {

        .file_format   = VMCU_FMT_IHEX,
        .endianness    = VMCU_ENDIAN_LITTLE,
        .byte_count    = 0x10,
        .words_mutable = true
    }
};

__thread vmcu_engine_t vmcu = {

    .rc              = default_engine.rc,
    .instruction_set = default_engine.instruction_set,

    .cb = {

        .alloc = default_engine.cb.alloc,
        .free  = default_engine.cb.free
    },

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
    },

    .writer = {

        .file_format   = default_engine.writer.file_format,
        .endianness    = default_engine.writer.endianness,
        .byte_count    = default_engine.writer.byte_count,
        .words_mutable = default_engine.writer.words_mutable
    }
};

void vmcu_engine_reset(void) {

    memcpy(&vmcu, &default_engine, sizeof(vmcu_engine_t));
}

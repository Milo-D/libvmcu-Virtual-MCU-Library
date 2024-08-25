#include <stdio.h>
#include <stdlib.h>
#include <assembler.h>
#include <disassembler.h>

int main(void) {

    vmcu_engine_t e;
    vmcu_engine_init(&e);
    e.assembler.expect_operand_structs = false;

    uint32_t addr = 0x0000;
    uint16_t muls_buf[4];

    addr += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R16, VMCU_REGISTER_R31);
    addr += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R31, VMCU_REGISTER_R16);
    addr += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R23, VMCU_REGISTER_R28);
    addr += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R30, VMCU_REGISTER_R19);

    for (uint32_t i = 0; i < 4; i++)
        printf("%08x: %04x\n", i, muls_buf[i]);

    unsigned int rc = 0;
    rc += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_NONE, VMCU_REGISTER_R19);
    rc += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R15,   VMCU_REGISTER_R31);
    rc += vmcu_asm(&e, &muls_buf[addr], VMCU_IID_MULS, VMCU_REGISTER_R16,   VMCU_REGISTER_R31 + 1);

    printf("rc: %d\n", rc);

    for (uint32_t i = 0; i < 4; i++)
        printf("%08x: %04x\n", i, muls_buf[i]);

    return EXIT_SUCCESS;
}

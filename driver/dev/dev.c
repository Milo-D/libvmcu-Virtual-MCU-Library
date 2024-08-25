#include <stdio.h>
#include <stdlib.h>
#include <assembler.h>
#include <disassembler.h>

int main(void) {

    vmcu_engine_t e;
    vmcu_engine_init(&e);
    e.assembler.expect_operand_structs = false;

    uint32_t addr = 0x0000;
    uint16_t mul_buf[4];

    addr += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_R0, VMCU_REGISTER_R4);
    addr += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_R2, VMCU_REGISTER_R16);
    addr += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_R4, VMCU_REGISTER_R28);
    addr += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_R6, VMCU_REGISTER_R14);

    for (uint32_t i = 0; i < 4; i++)
        printf("%08x: %04x\n", i, mul_buf[i]);

    unsigned int rc = 0;
    rc += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_NONE, VMCU_REGISTER_R4);
    rc += vmcu_asm(&e, &mul_buf[addr], VMCU_IID_MUL, VMCU_REGISTER_R1, VMCU_REGISTER_R31 + 1);

    printf("rc: %d\n", rc);

    for (uint32_t i = 0; i < 4; i++)
        printf("%08x: %04x\n", i, mul_buf[i]);

    return EXIT_SUCCESS;
}

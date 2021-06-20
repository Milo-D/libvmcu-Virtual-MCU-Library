/* Mnemonic (Datastructure) Header */

#ifndef VMCU_MNEMONIC_H
#define VMCU_MNEMONIC_H

typedef struct vmcu_mnemonic {

    char base    [7];

    char src     [9];
    char dest    [9];

    char comment[40];

} vmcu_mnemonic_t;

#endif

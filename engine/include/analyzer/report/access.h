/* Read-Write Access Header */

#ifndef VMCU_ACCESS_H
#define VMCU_ACCESS_H

typedef struct vmcu_access {

    /* larger segments */
    unsigned int registers : 1;
    unsigned int flash     : 1;
    unsigned int stack     : 1;
    unsigned int io        : 1;
    unsigned int ds        : 1;

    /* special pointers */
    unsigned int sp        : 1;
    unsigned int pc        : 1;

    /* status flags */
    unsigned int c_flag    : 1;
    unsigned int z_flag    : 1;
    unsigned int n_flag    : 1;
    unsigned int v_flag    : 1;
    unsigned int s_flag    : 1;
    unsigned int h_flag    : 1;
    unsigned int t_flag    : 1;
    unsigned int i_flag    : 1;

} vmcu_access_t;

#endif

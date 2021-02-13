/* Composed Disassembly Header */

#ifndef CDIS_H
#define CDIS_H

/*
 * Composed Disassembly Structure.
 *
 * This structure contains the actual representation of
 * the disassembled binary.
 * */

enum {

    CDIS_NO_ADDR = -1,
    CDIS_NO_OPC
};

typedef struct _composed_line {

    char *str;

    int32_t addr;
    uint32_t opc;

} composed_line_t;

typedef struct _composed_disassembly {

    int32_t size;
    composed_line_t *line;

} composed_disassembly_t;

extern struct _composed_disassembly* cdis_ctor(const int32_t size);
extern void cdis_dtor(struct _composed_disassembly *this);

extern void cdis_assign_line(struct _composed_line *this, int32_t addr, uint32_t opc, char *str);

#endif
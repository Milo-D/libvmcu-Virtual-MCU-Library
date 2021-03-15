/* Enumeration of Instruction Groups - Header */

#ifndef VMCU_GROUP_H
#define VMCU_GROUP_H

typedef enum {

    VMCU_GROUP_NONE = -1,

    VMCU_GROUP_MATH_LOGIC,
    VMCU_GROUP_SYS_CTRL,
    VMCU_GROUP_TRANSFER,
    VMCU_GROUP_FLOW,
    VMCU_GROUP_BIT,

} VMCU_GROUP;

#endif

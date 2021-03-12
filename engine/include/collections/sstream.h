/* StringStream Header */

#ifndef VMCU_SSTREAM_H
#define VMCU_SSTREAM_H

#define SSLEN 1024

// C Headers
#include <inttypes.h>

typedef struct vmcu_sstream {

    int length;
    char str[SSLEN];

} vmcu_sstream_t;

extern void vmcu_sstream_ctor(vmcu_sstream_t *this);

extern void vmcu_sstream_put(vmcu_sstream_t *this, const char *fmt, ...);
extern void vmcu_sstream_put04x(vmcu_sstream_t *this, const unsigned int decimal);
extern void vmcu_sstream_put02x(vmcu_sstream_t *this, const uint8_t decimal);

extern void vmcu_sstream_pad(vmcu_sstream_t *this, const int padding);
extern char* vmcu_sstream_alloc(const vmcu_sstream_t *this);

extern void vmcu_sstream_flush(vmcu_sstream_t *this);

#endif

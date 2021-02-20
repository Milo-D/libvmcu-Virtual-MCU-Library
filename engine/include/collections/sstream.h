/* StringStream Header */

#ifndef VMCU_SSTREAM_H
#define VMCU_SSTREAM_H

#define SSLEN 1024

// C Headers
#include <inttypes.h>

typedef struct sstream {

    int length;
    char str[SSLEN];

} sstream_t;

extern void sstream_ctor(sstream_t *this);

extern void sstream_put(sstream_t *this, const char *fmt, ...);
extern void sstream_put04x(sstream_t *this, const unsigned int decimal);
extern void sstream_put02x(sstream_t *this, const uint8_t decimal);

extern void sstream_pad(sstream_t *this, const int padding);
extern char* sstream_alloc(const sstream_t *this);

extern void sstream_flush(sstream_t *this);

#endif

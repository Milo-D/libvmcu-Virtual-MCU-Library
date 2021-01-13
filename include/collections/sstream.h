/* StringStream Header */

#ifndef SSTREAM_H
#define SSTREAM_H

#define SSLEN 1024

// C Headers
#include <inttypes.h>

typedef struct _sstream {

    int length;
    char str[SSLEN];

} sstream_t;

extern void sstream_ctor(struct _sstream *this);

extern void sstream_put(struct _sstream *this, const char *fmt, ...);
extern void sstream_put04x(struct _sstream *this, const unsigned int decimal);
extern void sstream_put02x(struct _sstream *this, const uint8_t decimal);

extern void sstream_pad(struct _sstream *this, const int padding);
extern char* sstream_alloc(const struct _sstream *this);

extern void sstream_flush(struct _sstream *this);

#endif

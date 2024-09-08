#include <stdio.h>
#include <stdbool.h>

#include <lib/engine.h>
#include "ihex.h"

typedef struct data_record {

    uint8_t  bc;
    uint32_t addr;
    uint8_t  bytes[255];
    FILE*    stream;

} data_record_t;

static void write_record_00(FILE* stream, uint8_t bc, uint16_t addr, char* payload, uint8_t cks) {

    fprintf(stream, ":%02X%04X00%s%02X\n", bc, addr, payload, cks);
}

static void write_record_01(FILE* stream) {

    fprintf(stream, ":00000001FF\n");
}

static void write_record_04(FILE* stream, const uint16_t extended_addr, uint8_t cks) {

    fprintf(stream, ":02000004%04X%02X\n", extended_addr, cks);
}

static void data_record_open(data_record_t* drec, FILE* stream) {

    drec->bc     = 0;
    drec->addr   = 0;
    drec->stream = stream;
}

static void data_record_new(data_record_t* drec, const uint32_t base_addr) {

    const uint16_t new_upper16 = ((base_addr * 2) & 0xffff0000) >> 16;

    if (new_upper16 != ((drec->addr & 0xffff0000) >> 16)) {

        uint8_t checksum;

        checksum  = 0x02;
        checksum += 0x00;
        checksum += 0x00;
        checksum += 0x04;
        checksum += new_upper16 >> 8;
        checksum += new_upper16 & 0xff;
        checksum  = ~checksum + 1;

        write_record_04(drec->stream, new_upper16, checksum);
    }

    drec->bc   = 0;
    drec->addr = base_addr * 2;
}

static void data_record_add(data_record_t* drec, const uint16_t word) {

    if (vmcu.writer.endianness == VMCU_ENDIAN_LITTLE) {

        drec->bytes[ drec->bc + 0 ] = (word & 0x00ff) >> 0;
        drec->bytes[ drec->bc + 1 ] = (word & 0xff00) >> 8;

    } else {

        drec->bytes[ drec->bc + 0 ] = (word & 0xff00) >> 8;
        drec->bytes[ drec->bc + 1 ] = (word & 0x00ff) >> 0;
    }

    drec->bc += 2;
}

static void data_record_submit(data_record_t* drec) {

    char    payload[(255 * 2) + 1];
    uint8_t checksum;
    size_t  pos = 0;

    if (drec->bc == 0)
        return;

    checksum  = drec->bc;
    checksum += (drec->addr & 0xff00) >> 8;
    checksum += (drec->addr & 0x00ff) >> 0;
    checksum += 0x00;

    for (uint8_t i = 0; i < drec->bc; i++) {

        pos += sprintf(payload + pos, "%02X", drec->bytes[i]);
        checksum += drec->bytes[i];
    }

    checksum = ~checksum + 1;
    write_record_00(drec->stream, drec->bc, drec->addr, payload, checksum);
}

static void data_record_close(data_record_t* drec) {

    write_record_01(drec->stream);
}

static inline bool data_record_is_full(const data_record_t* drec) {

    return (drec->bc >= vmcu.writer.byte_count);
}

static inline uint32_t data_record_top(const data_record_t* drec) {

    return (drec->addr + drec->bc);
}

static void write_ihex_records(FILE* stream, vmcu_word_t* words, const uint32_t n) {

    data_record_t drec;

    data_record_open(&drec, stream);
    data_record_new(&drec, words[0].addr); // TODO: check input address range (overflow when multiplied)
    data_record_add(&drec, words[0].raw);

    for (uint32_t i = 1; i < n; i++) {

        vmcu_word_t* cur, *prev;

        cur  = &words[i];
        prev = &words[i - 1];

        if ((data_record_top(&drec) & 0xffff) == 0x0000) {

            data_record_submit(&drec);
            data_record_new(&drec, cur->addr);

        } else if (cur->addr - prev->addr != 1 || data_record_is_full(&drec)) {

            data_record_submit(&drec);
            data_record_new(&drec, cur->addr);
        }

        data_record_add(&drec, cur->raw);
    }

    data_record_submit(&drec);
    data_record_close(&drec);
}

vmcu_rc_t write_ihex(FILE* stream, vmcu_word_t* words, const uint32_t n_words) {

    if (vmcu.writer.byte_count == 0 || vmcu.writer.byte_count % 2 != 0)
        return VMCU_RC_BAD_ENGINE;

    if (n_words != 0)
        write_ihex_records(stream, words, n_words);
    
    return VMCU_RC_OK;
}
